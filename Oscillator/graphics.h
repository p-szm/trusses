//
//  graphics.h
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Oscillator__graphics__
#define __Oscillator__graphics__

#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <sstream>
#include <vector>

#include "particle.h"
#include "physics.h"

extern int window_width;
extern int window_height;

void glut_print (float x, float y, std::string s);
void display_fps(double dt);
void display_energy();
void draw_gravity_indicator();

void display();
void reshape(int width, int height);
void mouse_click (int button, int state, int x, int y);
void key_pressed(unsigned char key, int x, int y);
void idle();
void mouse_passive(int x, int y);
void mouse_drag(int x, int y);

void draw_particle(const Particle& p);
void draw_vector(Vector2d v, Vector2d start, float r, float g, float b);
void draw_bar(Bar& b);

double abs_d(double x);

#endif /* defined(__Oscillator__graphics__) */
