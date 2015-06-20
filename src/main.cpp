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
#include "game.h"
#include "save.h"
#include <cstdlib>

// TODO: Velocities are wrong
// TODO: Important! Check strain signs
// TODO: Mouse at the edge scrolls the world
// TODO: Set idle to null when in the editor mode

int main(int argc, char * argv[])
{
    // Seed the random number generator
    srand( (unsigned int) time(0) );
    
    // Initialise the GLUT window and register the callbacks
    setup_graphics(argc, argv);
    register_callbacks();
    game.enter_editor();
    
    // Load the structure from a file if the -f option is used
    if (argc == 3 && std::string(argv[1]) == "-f")
    {
        if (load(argv[2]))
        {
            std::cout << "Could not load the file" << std::endl;
            std::exit(1);
        }
    }
    
    glutMainLoop();
    
    return 0;
}
