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

// * * * * * * * * * * //
enum bars_color_mode_t {STRAIN_C = 0, TEMP_C};

// * * * * * * * * * * //
void editor_display();
void simulation_display();
void reshape(int width, int height);
void editor_idle();
void simulation_idle();
void set_bars_color_mode(bars_color_mode_t mode);

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
