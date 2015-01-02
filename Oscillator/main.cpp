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
    glutCreateWindow("GLUT test");
    
    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse_click);
    glutKeyboardFunc(key_pressed);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouse_passive);
    glutMotionFunc(mouse_drag);
    glutSpecialFunc(special_key_down);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glPointSize(5.0);
    glLineWidth(1.0);
    
    microsecond_time(t);
    delta_t = (t - prev_t)/1000000.0;
    
    create_buttons();
    
    glutMainLoop();
    
    return 0;
}
