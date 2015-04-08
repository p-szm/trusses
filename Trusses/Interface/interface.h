//
//  interface.h
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__interface__
#define __Trusses__interface__

#include "math.h"
#include <vector>

class Tool;

// * * * * * * * * * * //
void key_down_function(unsigned char key, int x, int y);
void special_key_up(int key, int x, int y);
void special_key_down(int key, int x, int y);
void mouse_click(int button, int state, int x, int y);
void mouse_passive(int x, int y);
void mouse_drag(int x, int y);
void pause_simulation();
void resume_simulation();
bool simulation_is_paused();
void register_callbacks();
void idle();

// * * * * * * * * * * //
// Some helper functions to convert between the coordinates
Vector2d px_to_m(const Vector2d& v);
double px_to_m(double d);
Vector2d px_to_ui(const Vector2d& v);
double px_to_ui_x(double d);
double px_to_ui_y(double d);

struct Arrows
{
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
};

// * * * * * * * * * * //
extern Arrows arrows;
extern double grid_dist_px;
extern bool command_mode;
extern bool full_screen;
extern Tool* current_tool;

#endif /* defined(__Trusses__interface__) */
