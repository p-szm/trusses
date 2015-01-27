//
//  interface.h
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__interface__
#define __Trusses__interface__

#include <iostream>
#include "math.h"

inline double px_to_m(double d);

//////

void key_pressed(unsigned char key, int x, int y);
void special_key_down(int key, int x, int y);
void mouse_passive(int x, int y);
void mouse_click (int button, int state, int x, int y);
void mouse_drag(int x, int y);

/////

extern bool snap;
extern bool snapped;
extern int selected_particle_id;
extern int highlighted_particle_id;
extern Vector2d world_centre;
extern double scale;
extern bool drawing_wall;
extern std::vector<Vector2d> wall_points;
extern bool command_mode;
extern Vector2d mouse_pos;
extern bool full_screen;

#endif /* defined(__Trusses__interface__) */
