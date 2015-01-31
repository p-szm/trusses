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

// * * * * * * * * * * //
void key_function(unsigned char key, int x, int y);
void special_key_function(int key, int x, int y);
void mouse_function(int button, int state, int x, int y);
void mouse_passive_function(int x, int y);
void mouse_drag_function(int x, int y);
void start_drawing_wall();
void stop_drawing_wall();

// * * * * * * * * * * //
extern double scale;
extern Vector2d world_centre;
extern bool snap;
extern bool snapped;
extern std::vector<int> selected_particles_ids;
extern bool drawing_wall;
extern std::vector<Vector2d> wall_points;
extern Vector2d mouse_pos;
extern bool command_mode;
extern bool full_screen;

// * * * * * * * * * * //
inline double px_to_m(double d) {return d / scale;} // Defined here, because clang complained

#endif /* defined(__Trusses__interface__) */
