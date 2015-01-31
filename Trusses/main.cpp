//
//  main.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

#include "graphics.h"
#include "physics.h" // for time
#include "button.h"
#include "interface.h"
#include "temporary_label.h" // Temporary

// TODO: Add actual strings, which don't have any tension below their minimum length
// TODO: Implement blobs
// TODO: There is some bug when removing particles, then saving and importing
// TODO: Vlocities are wrong
// TODO: Important! Check strain signs
// TODO: Mouse at the edge scrolls the world

int main(int argc, char * argv[])
{
    srand( (unsigned int) time(0) );
    
    // Initiallize GLUT
    glutInit(&argc, argv);
    
    // Setup for the new window
    glutInitWindowPosition(160, 80);
    glutInitWindowSize(window_width, window_height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    // Create a window
    glutCreateWindow("Truss simulation");
    
    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse_function);
    glutKeyboardFunc(key_function);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouse_passive_function);
    glutMotionFunc(mouse_drag_function);
    glutSpecialFunc(special_key_function);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glPointSize(5.0);
    glLineWidth(1.0);
    glClearColor(0.12, 0.12, 0.12, 1.0);
    
    microsecond_time(t);
    delta_t = 0.02; // Start with something
    
    create_buttons();
    
    TempLabel::create("Editor mode - you can draw the structure", Vector2d(-110, window_height/2 - 20), 5000000); // 5s
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     
    glutMainLoop();
    
    return 0;
}
