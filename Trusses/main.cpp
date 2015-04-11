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
#include "interface.h"

// TODO: Velocities are wrong
// TODO: Important! Check strain signs
// TODO: Mouse at the edge scrolls the world
// TODO: Clean up key_down_function
// TODO: Set idle to null when in the editor mode

int main(int argc, char * argv[])
{
    // Seed the random number generator
    srand( (unsigned int) time(0) );
    
    // Initialise the GLUT window and register the callbacks
    setup_graphics(argc, argv);
    register_callbacks();
    
    glutMainLoop();
    
    return 0;
}
