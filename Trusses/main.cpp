//
//  main.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>

#include "graphics.h"
#include "physics.h" // for time
#include "button.h"
#include "interface.h"
#include "temporary_label.h"

// TODO: Velocities are wrong
// TODO: Important! Check strain signs
// TODO: Mouse at the edge scrolls the world
// TODO: Simplify the collision with walls
// TODO: Clean up key_down_function
// TODO: Particles can penetrate the corners of the walls
// TODO: Check every SlotMap container if it's not being
// iterated over without using iterators

int main(int argc, char * argv[])
{
    // Seed the random number generator
    srand( (unsigned int) time(0) );
    
    // Initialise the GLUT window and register the callbacks
    setup_graphics(argc, argv);
    register_callbacks();
    
    // Initialises time
    microsecond_time(t);
    delta_t = 0.02; // Start with something
    
    create_buttons_editor();
    TempLabel::create("Editor mode - you can draw the structure. Press \"p\" when you are done", 0, 1.0, 0, -TOP_MARGIN, MODE_LABEL_TIME);
    
    glutMainLoop();
    
    return 0;
}
