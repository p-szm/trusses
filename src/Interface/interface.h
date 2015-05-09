//
//  interface.h
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__interface__
#define __Trusses__interface__

#include "various_math.h"
#include <vector>

class Tool;
class Interpreter;

void key_down(unsigned char key, int x, int y);
void command_key_down(unsigned char key, int x, int y);
void special_key_up(int key, int x, int y);
void special_key_down(int key, int x, int y);
void mouse_click(int button, int state, int x, int y);
void mouse_passive(int x, int y);
void mouse_drag(int x, int y);
void register_callbacks();
void idle();

// Some helper functions to convert between the coordinates
Vector2d px_to_m(const Vector2d& v);
double px_to_m(double d);
Vector2d px_to_ui(const Vector2d& v);
double px_to_ui_x(double d);
double px_to_ui_y(double d);

struct Arrows
{
    Arrows();
    bool left;
    bool right;
    bool up;
    bool down;
};

// * * * * * * * * * * //
extern Arrows arrows;
extern bool command_mode;
extern Tool* current_tool;
extern Interpreter interpreter;

#endif /* defined(__Trusses__interface__) */
