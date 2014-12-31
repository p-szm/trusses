//
//  graphics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "graphics.h"
#include <sstream>

int window_width = 500;
int window_height = 500;

extern std::vector<Particle> particles;
extern std::vector<Bar> bars;

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
    
    // Draw the particles
    for (int i = 0; i < particles_number; i++)
    {
        draw_particle(particles[i]);
    }
    
    // Draw the bars
    for (int i = 0; i < bars.size(); i++)
    {
        draw_bar(bars[i]);
    }
    
    // Draw the vectors
    for (int i = 0; i < particles_number; i++)
    {
        if (!particles[i].fixed)
        {
            // Draw the velocity vector
            draw_vector(particles[i].velocity, particles[i].position, 0.0, 0.5, 0.0);
            
            // Draw the acceleration vector
            draw_vector(particles[i].acceleration, particles[i].position, 0.5, 0.0, 0.0);
        }
    }
    
    display_fps(delta_t);
    display_energy();
    
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
    
    /*if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        for (int i = 0; i < particles_number; i++)
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
    }*/
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Create the first particle
        if (particles_number == 0)
        {
            Particle new_p = Particle::create(x_coord, y_coord, false);
            particles.push_back(new_p);
        }
        // Particles already exist
        else
        {
            // See if any particle was clicked
            int clicked_particle_id = -1;
            for (int i = 0; i < particles_number; i++)
            {
                Vector2d p_pos = particles[i].position;
                if (abs_d(x_coord - p_pos.x) < 0.1 && abs_d(y_coord - p_pos.y) < 0.1)
                {
                    clicked_particle_id = particles[i].id;
                    break;
                }
            }
            
            // If a particle was clicked and one was already selected
            if (clicked_particle_id != -1 && selected_particle_id != -1)
            {
                Bar new_b = Bar::create(clicked_particle_id, selected_particle_id);
                bars.push_back(new_b);
                selected_particle_id = -1; // Unselect
            }
            
            // If a particle was clicked and no particle was selected
            else if (clicked_particle_id != -1 && selected_particle_id == -1)
            {
                selected_particle_id = clicked_particle_id;
            }
            
            // If a particle was selected but no particle clicked
            else if (clicked_particle_id == -1 && selected_particle_id != -1)
            {
                // Create a new particle
                Particle new_p = Particle::create(x_coord, y_coord, false);
                particles.push_back(new_p);
                
                // Create a new bar
                Bar new_b = Bar::create(selected_particle_id, new_p.id);
                bars.push_back(new_b);
                
                selected_particle_id = -1;
            }
        }
        
        glutPostRedisplay();
    }
    
    /*else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        Particle new_p = Particle::create(x_coord, y_coord, true);
        particles.push_back(new_p);
        
        particles.push_back(new_p);
        
        glutPostRedisplay();
    }*/
}

void key_pressed(unsigned char key, int x, int y)
{
    if (key == 'r')
    {
        particles.clear();
        bars.clear();
        particles_number = 0;
        bars_number = 0;
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
    
    for (int i = 0; i < particles_number; i++)
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

void draw_particle(const Particle& p)
{
    Vector2d pos = p.position;
    if (p.highlight)
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
    
    std::stringstream s;
    s << p.id;
    
    Vector2d vel = p.velocity;
    
    glut_print(pos.x - 0.04*vel.norm().x, pos.y - 0.04*vel.norm().y, s.str());
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

void draw_bar(Bar& b)
{
    Vector2d start = particles[b.p1_id].position;
    Vector2d end = particles[b.p2_id].position;
    Vector2d mid(0.5*(start.x + end.x), 0.5*(start.y + end.y));
    
    glColor3f(1.0, 1.0, 1.0);
    
    glBegin(GL_LINES);
    glVertex2f(start.x, start.y);
    glVertex2f(end.x, end.y);
    glEnd();
    
    std::stringstream s;
    s.precision(3);
    s << b.length();
    
    glut_print(mid.x, mid.y, s.str());
    
    s.str("");
    s << b.extension_rate();
    
    glut_print(mid.x, mid.y - 0.05, s.str());
}