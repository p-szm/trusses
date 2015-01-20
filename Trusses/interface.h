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

inline Vector2d m_to_px(Vector2d v);
inline double m_to_px(double d);

inline Vector2d px_to_m(Vector2d v);
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
extern Vector2d snapped_point;
extern int selected_particle_id;
extern int highlighted_particle_id;

extern Vector2d world_centre;
extern double scale;

#endif /* defined(__Trusses__interface__) */
