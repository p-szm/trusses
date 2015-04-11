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
#include "physics.h"
#include "save.h"
#include "interpreter.h"
#include "temporary_label.h"
#include "mouse.h"
#include "bars_tool.h"
#include "drag_tool.h"
#include "window.h"
#include "game.h"
#include "settings.h"

// * * * * * * * * * * //
Arrows arrows;
bool command_mode = false;
Tool* current_tool = 0;
Interpreter interpreter;

// * * * * * * * * * * //
Vector2d px_to_m(const Vector2d& v) {return Vector2d(px_to_m(v.x), px_to_m(v.y));}
double px_to_m(double d) {return d / window.scale;}
Vector2d px_to_ui(const Vector2d& v) {return Vector2d(px_to_ui_x(v.x), px_to_ui_y(v.y));}
double px_to_ui_x(double d) {return d / (window.width/2.0);}
double px_to_ui_y(double d) {return d / (window.height/2.0);}

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
            settings.toggle(GRAVITY);
        else if (key == 'o')
            create_cloth(20, 0.5, Vector2d(0.0, 0.0), false);
        else if (key == 27)
        {
            Tool::set(current_tool, NULL);
            game.reset();
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
            if (settings.bars_color_mode == STRAIN_C)
                settings.bars_color_mode = TEMP_C;
            else if (settings.bars_color_mode == TEMP_C)
                settings.bars_color_mode = STRAIN_C;
        }
        else if (key == 'f')
        {
            window.full_screen = !window.full_screen;
            if (window.full_screen)
            {
                glutFullScreen();
            }
            else
            {
                glutReshapeWindow(1100, 750);
                glutPositionWindow(100, 80);
            }
        }
        else if (key == 'b')
            settings.toggle(BOUNDING_BOXES);
        else if (key == 't')
            settings.toggle(TRIANGULATION);
        else if (key == 's')
            settings.toggle(PARTICLES);
        else if (key == 'p')
        {
            if (game.simulation_is_paused())
                game.resume_simulation();
            else
                game.pause_simulation();
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
        arrows.up = true;
    if (key == GLUT_KEY_DOWN)
        arrows.down = true;
    if (key == GLUT_KEY_LEFT)
        arrows.left = true;
    if (key == GLUT_KEY_RIGHT)
        arrows.right = true;
    
    if (command_mode)
    {
        // If down arrow is pressed
        if (arrows.down && current_cmd > 0)
            current_cmd--;
        // If up arrow is pressed
        else if (arrows.up && current_cmd < commands.size() - 1)
            current_cmd++;
    }
}

void special_key_up(int key, int x, int y)
{
    mouse.update(x, y);
    
    // Remove the arrow from the list of pressed arrows
    if (key == GLUT_KEY_UP)
        arrows.up = false;
    if (key == GLUT_KEY_DOWN)
        arrows.down = false;
    if (key == GLUT_KEY_LEFT)
        arrows.left = false;
    if (key == GLUT_KEY_RIGHT)
        arrows.right = false;
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
    game.update();
    update_labels();
    window.update_centre(arrows, game.dt_s());
    glutPostRedisplay();
}
