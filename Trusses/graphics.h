//
//  graphics.h
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__graphics__
#define __Trusses__graphics__

// * * * * * * * * * * //
void display();
void reshape(int width, int height);
void idle();

// * * * * * * * * * * //
enum bars_color_mode_t {STRAIN_C = 0, TEMP_C};
extern bars_color_mode_t bars_color_mode;

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

#endif /* defined(__Trusses__graphics__) */
