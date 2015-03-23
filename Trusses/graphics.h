//
//  graphics.h
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__graphics__
#define __Trusses__graphics__
#include "math.h"

#define COMMAND_LINE_SIZE 30
#define BOTTOM_MARGIN 20
#define TOP_MARGIN 25
#define WORLD_VIEW -window_width/(2.0*world.scale) + world.centre.x, window_width/(2.0*world.scale) + world.centre.x, -window_height/(2.0*world.scale) + world.centre.y, window_height/(2.0*world.scale) + world.centre.y
#define UI_VIEW -1, 1, -1, 1

#define BLACK 0.0, 0.0, 0.0
#define DARK_GREY 0.3, 0.3, 0.3
#define GREY 0.5, 0.5, 0.5
#define WHITE 1.0, 1.0, 1.0

#define RED 1.0, 0.0, 0.0
#define LIME 0.0, 1.0, 0.0
#define BLUE 0.0, 0.0, 1.0

#define YELLOW 1.0, 1.0, 0.0
#define AQUA 0.0, 1.0, 0.0
#define FUCHSIA 1.0, 0.0, 1.0

#define MAROON 0.5, 0.0, 0.0
#define GREEN 0.0, 0.5, 0.0
#define NAVY 0.0, 0.0, 0.5

#define OLIVE 0.5, 0.5, 0.0
#define TEAL 0.0, 0.5, 0.5
#define PURPLE 0.5, 0.0, 0.5

#define GOLD 1.0, 0.84, 0.0

// * * * * * * * * * * //
enum bars_color_mode_t {STRAIN_C = 0, TEMP_C};

// * * * * * * * * * * //
void display();
void reshape(int width, int height);
void set_bars_color_mode(bars_color_mode_t mode);
void draw_rectangle(Vector2d p1, Vector2d p2, bool filled);
void draw_circle(Vector2d centre, double r, unsigned int n_points);
void draw_cross(Vector2d pos, int size_px);
void draw_point(Vector2d pos);

// * * * * * * * * * * //
void setup_graphics(int argc, char * argv[]);

// * * * * * * * * * * //
extern int window_width;
extern int window_height;
extern bool ids;
extern bool velocities;
extern bool accelerations;
extern bool lengths;
extern bool extensions;
extern bool coords;
extern bool fancy_bars;
extern bool show_particles;
extern bars_color_mode_t bars_color_mode;

#endif /* defined(__Trusses__graphics__) */
