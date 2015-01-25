//
//  graphics.h
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__graphics__
#define __Trusses__graphics__

#include "particle.h"
#include "wall.h"
#include "button.h"
#include "bar.h"

void glut_print (float x, float y, std::string s, bool px = false);
void display_fps(double dt);
void display_temperature(double temp);
void draw_gravity_indicator();

void display();
void reshape(int width, int height);
void idle();

void draw_particle(Particle& p);
void draw_vector(Vector2d v, Vector2d start, float r, float g, float b);
void draw_bar(const Bar& b);
void draw_coords();

void draw_rectangle(Vector2d p1, Vector2d p2, bool px);

void draw_wall(const Wall& w);
void draw_button(const Button& b);

/////

inline Vector2d metres_to_gl_coords(const Vector2d& v);
inline double metres_to_gl_coords_x(double d);
inline double metres_to_gl_coords_y(double d);

inline Vector2d px_to_gl_coords(const Vector2d& v);
inline double px_to_gl_coords_x(double d);
inline double px_to_gl_coords_y(double d);

/////

extern bool ids;
extern bool velocities;
extern bool accelerations;
extern bool lengths;
extern bool extensions;
extern bool coords;
extern bool snap;

extern double scale;

extern bool drawing_wall;

extern int window_width;
extern int window_height;

#endif /* defined(__Trusses__graphics__) */
