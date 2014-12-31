//
//  graphics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "graphics.h"

int window_width = 500;
int window_height = 500;

extern std::vector<Particle> particles;

void glut_print (float x, float y, std::string s)
// Prints string at location (x,y) in a bitmap font
{
    unsigned short i;
    
    glRasterPos2f(x, y);
    for (i = 0; i < s.length(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
}

void display_fps(double dt)
{
    std::ostringstream s;
    s << "fps: " << int(1/dt);
    glut_print(-0.9, -0.9, s.str());
}

void display_energy()
{
    std::ostringstream s;
    s.precision(3);
    s << "Energy: " << energy(particles);
    glut_print(0.6, -0.9, s.str());
}

void display()
{
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Switch to the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < particles.size(); i++)
    {
        Vector2d pos = particles[i].position;
        glVertex2f(pos.x, pos.y);
    }
    glEnd();
    
    for (int i = 0; i < particles.size(); i++)
    {
        Vector2d pos = particles[i].position;
        if (particles[i].highlight)
        {
            glPointSize(10);
            glBegin(GL_POINTS);
            glVertex2f(pos.x, pos.y);
            glEnd();
        }
        else
        {
            glPointSize(5);
            glBegin(GL_POINTS);
            glVertex2f(pos.x, pos.y);
            glEnd();
        }
    }
    glEnd();
    
    display_fps(delta_t);
    display_energy();
    
    for (int i = 0; i < particles.size(); i++)
    {
        if (!particles[i].fixed)
        {
            // Draw the velocity vector
            draw_vector(particles[i].velocity, particles[i].position, 0.0, 0.5, 0.0);
            
            // Draw the acceleration vector
            draw_vector(particles[i].acceleration, particles[i].position, 0.5, 0.0, 0.0);
        }
    }
    
    glutSwapBuffers();
    
    glFlush();
}

void reshape(int width, int height)
{
    // Define the viewport transformation
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
}

void mouse_click (int button, int state, int x, int y)
{
    // GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON
    // GLUT_UP or GLUT_DOWN
    
    float x_coord = x * 2.0 / window_width - 1.0;
    float y_coord = -y * 2.0 / window_height + 1.0;
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        for (int i = 0; i < particles.size(); i++)
        {
            Vector2d p_pos = particles[i].position;
            if (abs_d(x_coord - p_pos.x) < 0.1 && abs_d(y_coord - p_pos.y) < 0.1)
            {
                if (particles[i].fixed)
                    particles[i].unfix();
                else
                    particles[i].fix();
                return;
            }
        }
    }
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        Particle new_p = Particle::create(x_coord, y_coord, false);
        particles.push_back(new_p);
        
        glutPostRedisplay();
    }
    
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        Particle new_p = Particle::create(x_coord, y_coord, true);
        particles.push_back(new_p);
        
        particles.push_back(new_p);
        
        glutPostRedisplay();
    }
}

void key_pressed(unsigned char key, int x, int y)
{
    if (key == 'r')
    {
        particles.clear();
        std::cout << "Clear" << std::endl;
    }
    else if (key == 'g')
    {
        gravity = !gravity;
        std::cout << "Gravity: " << gravity << std::endl;
    }
    else if (key == 27)
    {
        std::exit(0);
    }
    
    glutPostRedisplay();
}

void idle()
{
    update_time();
    update_position(particles);
    
    glutPostRedisplay();
}

double abs_d(double x)
{
    if (x >= 0.0)
        return x;
    else
        return -x;
}

void mouse_passive(int x, int y)
{
    float x_coord = x * 2.0 / window_width - 1.0;
    float y_coord = -y * 2.0 / window_height + 1.0;
    
    for (int i = 0; i < particles.size(); i++)
    {
        Vector2d p_pos = particles[i].position;
        if (abs_d(x_coord - p_pos.x) < 0.1 && abs_d(y_coord - p_pos.y) < 0.1)
        {
            particles[i].highlight = true;
        }
        else
            particles[i].highlight = false;
    }
}

void draw_vector(Vector2d v, Vector2d start, float r, float g, float b)
{
    glColor3f(r, g, b);
    
    glBegin(GL_LINES);
    glVertex2f(start.x, start.y);
    glVertex2f(start.x + v.x, start.y + v.y);
    glEnd();
    
    glPointSize(5);
    
    glBegin(GL_POINTS);
    glVertex2f(start.x + v.x, start.y + v.y);
    glEnd();
    
    glColor3f(1.0, 1.0, 1.0);
}