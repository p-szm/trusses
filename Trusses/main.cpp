//
//  main.cpp
//  Oscillator
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
#include "temporary_label.h" // Temporary

// TODO: Add actual strings, which don't have any tension below their minimum length
// TODO: There is some bug when removing particles, then saving and importing
// TODO: Velocities are wrong
// TODO: Important! Check strain signs
// TODO: Mouse at the edge scrolls the world

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
    TempLabel::create("Editor mode - you can draw the structure. Press \"p\" when you are done", 0, 1.0, 0, -25, 5000000); // 5s
    
    glutMainLoop();
    
    return 0;
}
