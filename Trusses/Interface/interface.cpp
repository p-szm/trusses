//
//  interface.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "interface.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#endif
#include <cstdlib>

#include "button.h"
#include "graphics.h"
#include "physics.h"
#include "save.h"
#include "interpreter.h"
#include "temporary_label.h"
#include "mouse.h"
#include "bars_tool.h"
#include "drag_tool.h"

// * * * * * * * * * * //
World world;
double grid_dist_px = 30.0; // In pixels
const double scroll_speed = 10; // px/iteration
bool command_mode = false;
bool full_screen = false;
bool simulation_paused = true;
std::vector<bool> arrows(4); // left, right, up, down
Tool* current_tool = new BarsTool;
Interpreter interpreter;

// * * * * * * * * * * //
Vector2d px_to_m(const Vector2d& v) {return Vector2d(px_to_m(v.x), px_to_m(v.y));}
double px_to_m(double d) {return d / world.scale;}
Vector2d px_to_ui(const Vector2d& v) {return Vector2d(px_to_ui_x(v.x), px_to_ui_y(v.y));}
double px_to_ui_x(double d) {return d / (window_width/2.0);}
double px_to_ui_y(double d) {return d / (window_height/2.0);}

// * * * * * * * * * * //
void key_down_function(unsigned char key, int x, int y)
{
    mouse.update(x, y);
    
    if (command_mode)
    {
        if (key == 27)
        {
            command_mode = false;
            commands.back() = "";
        }
        else if (key == 13)
        {
            command_mode = false;
            std::string cmd_to_execute = commands[commands.size() - current_cmd - 1];
            if (cmd_to_execute != "")
            {
                interpreter.interpret(cmd_to_execute);
                commands.back() = cmd_to_execute;
            }
            current_cmd = 0;
        }
        else if (key == 127 || key == 8)
        {
            if (commands.back().size() > 0 && current_cmd == 0)
                commands.back() = commands.back().substr(0, commands.back().size()-1);
        }
        else
        {
            if (current_cmd == 0)
                commands.back() += key;
        }
    }
    else
    {
        if (key == 'g')
            gravity = !gravity;
        else if (key == 'o')
            create_cloth(20, 0.5, Vector2d(0.0, 0.0), false);
        else if (key == 27)
        {
            Tool::set(current_tool, NULL);
            reset_everything();
            std::exit(0);
        }
        else if (key == 13)
        {
            command_mode = true;
            if (commands.size() == 0 || commands.back() != "")
                commands.push_back("");
        }
        else if (key == 'c')
        {
            if (bars_color_mode == STRAIN_C)
                set_bars_color_mode(TEMP_C);
            else if (bars_color_mode == TEMP_C)
                set_bars_color_mode(STRAIN_C);
        }
        else if (key == 'f')
        {
            full_screen = !full_screen;
            if (full_screen)
            {
                glutFullScreen();
                // TODO: Labels have to update their position
            }
            else
            {
                glutReshapeWindow(1100, 750);
                glutPositionWindow(100, 80);
            }
        }
        else if (key == 'b')
            fancy_bars = !fancy_bars;
        else if (key == 's')
            show_particles = !show_particles;
        else if (key == 'p')
        {
            if (simulation_is_paused())
                resume_simulation();
            else
                pause_simulation();
        }
    }
    
    current_tool->key_down(key);
    glutPostRedisplay();
}

void special_key_down(int key, int x, int y)
{
    mouse.update(x, y);
    
    // Remember which arrows were pressed
    if (key == GLUT_KEY_UP)
        arrows[2] = true;
    if (key == GLUT_KEY_DOWN)
        arrows[3] = true;
    if (key == GLUT_KEY_LEFT)
        arrows[0] = true;
    if (key == GLUT_KEY_RIGHT)
        arrows[1] = true;
    
    if (command_mode)
    {
        // If down arrow is pressed
        if (!arrows[0] && !arrows[1] && !arrows[2] && arrows[3] && current_cmd > 0)
            current_cmd--;
        // If up arrow is pressed
        else if (!arrows[0] && !arrows[1] && arrows[2] && !arrows[3] && current_cmd < commands.size() - 1)
            current_cmd++;
    }
    else
    {
        if (arrows[0])
            world.centre.x -= px_to_m(scroll_speed);
        if (arrows[1])
            world.centre.x += px_to_m(scroll_speed);
        if (arrows[2])
            world.centre.y += px_to_m(scroll_speed);
        if (arrows[3])
            world.centre.y -= px_to_m(scroll_speed);
    }
}

void special_key_up(int key, int x, int y)
{
    mouse.update(x, y);
    
    // Remove the arrow from the list of pressed arrows
    if (key == GLUT_KEY_UP)
        arrows[2] = false;
    if (key == GLUT_KEY_DOWN)
        arrows[3] = false;
    if (key == GLUT_KEY_LEFT)
        arrows[0] = false;
    if (key == GLUT_KEY_RIGHT)
        arrows[1] = false;
}

void mouse_click(int button, int state, int x, int y)
{
    mouse.update(x, y);
    
    // Check if any button was pressed
    if (state == GLUT_DOWN)
    {
        int pressed_button_id = -1;
        for (int i = 0; i < buttons.size() && pressed_button_id == -1; i++)
        {
            if (buttons[i].highlighted_)
            {
                buttons[i].execute_action();
                return;
            }
        }
    }
    
    // Use the current tool
    current_tool->mouse_click(button, state);
    glutPostRedisplay();
}

void mouse_passive(int x, int y)
{
    mouse.update(x, y);
    highlight_buttons(mouse.pos_ui.x, mouse.pos_ui.y);
    current_tool->passive();
}

void mouse_drag(int x, int y)
{
    mouse.update(x, y);
    current_tool->drag();
}

void pause_simulation()
{
    Tool::set(current_tool, new BarsTool);
    simulation_paused = true;
    
    temp_labels.clear();
    TempLabel::create("Editor mode - you can draw the structure. Press \"p\" when you are done", 0, 1.0, 0, -TOP_MARGIN, MODE_LABEL_TIME);
    buttons.clear();
    create_buttons_editor();
    
    mouse.min_click_dist = 10;
}

void resume_simulation()
{
    Tool::set(current_tool, new DragTool);
    microsecond_time(t);
    simulation_paused = false;
    
    temp_labels.clear();
    TempLabel::create("Simulation mode - you can drag the joints", 0, 1.0, 0, -TOP_MARGIN, MODE_LABEL_TIME);
    buttons.clear();
    create_buttons_simulation();
    
    mouse.min_click_dist = 20;
}

// TODO: this function is useless, remove it
bool simulation_is_paused()
{
    return simulation_paused;
}

// * * * * * * * * * * //
void register_callbacks()
{
    glutMouseFunc(mouse_click);
    glutKeyboardFunc(key_down_function);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouse_passive);
    glutMotionFunc(mouse_drag);
    glutSpecialFunc(special_key_down);
    glutSpecialUpFunc(special_key_up);
}

void idle()
{
    update_time();
    update_labels();
    
    if (!simulation_is_paused())
    {
        // TODO: This is pretty accurate, but could be better
        // It was too fast by about 0.5s when I tested it on 8 minutes
        simulation_time += (t - prev_t);
        update_simulation();
    }
    glutPostRedisplay();
}
