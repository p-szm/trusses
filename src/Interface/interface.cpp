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

#include "button.h"
#include "save.h"
#include "interpreter.h"
#include "mouse.h"
#include "game.h"
#include "settings.h"
#include "tool.h"
#include <cstdlib>

Arrows::Arrows()
{
    left = false;
    right = false;
    up = false;
    down = false;
}

// * * * * * * * * * * //
Arrows arrows;
bool command_mode = false;
Tool* current_tool = 0;
Interpreter interpreter;

// * * * * * * * * * * //
Vector2d px_to_m(const Vector2d& v) {return Vector2d(px_to_m(v.x), px_to_m(v.y));}
double px_to_m(double d) {return d / window.get_scale();}
Vector2d px_to_ui(const Vector2d& v) {return Vector2d(px_to_ui_x(v.x), px_to_ui_y(v.y));}
double px_to_ui_x(double d) {return d / (window.width/2.0);}
double px_to_ui_y(double d) {return d / (window.height/2.0);}

// * * * * * * * * * * //
void command_key_down(unsigned char key, int x, int y)
{
    mouse.update(x, y);
    
    switch (key)
    {
        case 27:
        {
            interpreter.command = "";
            command_mode = false;
            glutKeyboardFunc(key_down);
            break;
        }
        case 13:
        {
            interpreter.interpret();
            interpreter.command = "";
            command_mode = false;
            glutKeyboardFunc(key_down);
            break;
        }
        case 127: case 8:
        {
            if (interpreter.command.size() > 0)
                interpreter.command.erase(interpreter.command.end()-1);
            break;
        }
        case 9:
        {
            command_mode = false;
            glutKeyboardFunc(key_down);
            break;
        }
        default:
        {
            interpreter.command += key;
            break;
        }
    }
    refresh_buttons();
    glutPostRedisplay();
}

void key_down(unsigned char key, int x, int y)
{
    mouse.update(x, y);

    switch (key)
    {
        case 'g':
        {
            settings.toggle(GRAVITY);
            break;
        }
        case 'o':
        {
            create_cloth(20, 0.5, Vector2d(0.0, 0.0), false);
            break;
        }
        case 27:
        {
            Tool::set(current_tool, NULL);
            game.reset();
            std::exit(0);
            break;
        }
        case 9:
        {
            command_mode = true;
            glutKeyboardFunc(command_key_down);
            break;
        }
        case 'f':
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
            break;
        }
        case 'b':
        {
            settings.toggle(BOUNDING_BOXES);
            break;
        }
        case 't':
        {
            settings.toggle(TRIANGULATION);
            break;
        }
        case 's':
        {
            settings.toggle(PARTICLES);
            break;
        }
        case 'p':
        {
            if (game.simulation_running())
                game.enter_editor();
            else
                game.enter_simulation();
            break;
        }
        default:
            current_tool->key_down(key);
            break;
    }
    
    refresh_buttons();
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
    refresh_buttons();
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
    refresh_buttons();
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
            if (buttons[i].is_highlighted())
            {
                buttons[i].execute_action();
                return;
            }
        }
    }
    
    // Use the current tool
    current_tool->mouse_click(button, state);
    refresh_buttons();
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
    glutKeyboardFunc(key_down);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouse_passive);
    glutMotionFunc(mouse_drag);
    glutSpecialFunc(special_key_down);
    glutSpecialUpFunc(special_key_up);
}

void idle()
{
    game.update();
    window.update(arrows, game.dt_s());
    glutPostRedisplay();
}
