//
//  graphics.h
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__graphics__
#define __Trusses__graphics__
#include "vector2d.h"

#define COMMAND_LINE_SIZE 30
#define WORLD_VIEW -window.width/(2.0*window.get_scale()) + window.centre.x, window.width/(2.0*window.get_scale()) + window.centre.x, -window.height/(2.0*window.get_scale()) + window.centre.y, window.height/(2.0*window.get_scale()) + window.centre.y
#define UI_VIEW -1, 1, -1, 1

#define BLACK 0.0, 0.0, 0.0
#define DARK_GREY 0.3, 0.3, 0.3
#define GREY 0.5, 0.5, 0.5
#define WHITE 1.0, 1.0, 1.0

#define RED 1.0, 0.0, 0.0
#define LIME 0.0, 1.0, 0.0
#define BLUE 0.0, 0.0, 1.0

#define YELLOW 1.0, 1.0, 0.0
#define AQUA 0.0, 1.0, 1.0
#define FUCHSIA 1.0, 0.0, 1.0

#define MAROON 0.5, 0.0, 0.0
#define GREEN 0.0, 0.5, 0.0
#define NAVY 0.0, 0.0, 0.5

#define OLIVE 0.5, 0.5, 0.0
#define TEAL 0.0, 0.5, 0.5
#define PURPLE 0.5, 0.0, 0.5

#define GOLD 1.0, 0.84, 0.0
#define ORANGE 1.0, 0.5, 0.0

// * * * * * * * * * * //
void display();
void reshape(int width, int height);
void draw_rectangle(Vector2d p1, Vector2d p2, bool filled);
void draw_circle(Vector2d centre, double r, unsigned int n_points, bool filled = false);
void draw_cross(Vector2d pos, int size_px);
void draw_point(Vector2d pos);
void glut_print (float x, float y, std::string s);
void setup_graphics(int argc, char * argv[]);

class Renderer;
extern Renderer renderer;

#endif /* defined(__Trusses__graphics__) */
