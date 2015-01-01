//
//  graphics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "graphics.h"
#include <sstream>

int window_width = 600;
int window_height = 500;

bool accelerations = false;
bool velocities = false;
bool lengths = false;
bool extension_rates = false;

int min_click_dist = 8;

extern std::vector<Particle> particles;
extern std::vector<Bar> bars;

void glut_print (float x, float y, std::string s)
// Prints string at location (x,y) in a bitmap font
{
    unsigned short i;
    
    glRasterPos2f(x*2.0/window_width, y*2.0/window_height);
    for (i = 0; i < s.length(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
}

void display_fps(double dt)
{
    std::ostringstream s;
    s << "fps: " << int(1/dt);
    glut_print(-window_width/2.0+30, -window_height/2.0+20, s.str());
}

void display_energy()
{
    std::ostringstream s;
    s.precision(3);
    s << "Energy: " << energy(particles);
    glut_print(window_width/2.0-60, -window_height/2.0+20, s.str());
}

void draw_gravity_indicator()
{
    std::string s;
    if (gravity)
        s = "Gravity ON";
    else
       s = "Gravity OFF";
    glut_print(-window_width/2.0+20, window_height/2.0-20, s);
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
            if (velocities)
                draw_vector(particles[i].velocity, particles[i].position, 0.0, 0.5, 0.0);
            
            // Draw the acceleration vector
            if (accelerations)
                draw_vector(particles[i].acceleration, particles[i].position, 0.5, 0.0, 0.0);
        }
    }
    
    display_fps(delta_t);
    display_energy();
    draw_gravity_indicator();
    
    glutSwapBuffers();
    glFlush();
}

void reshape(int width, int height)
{
    // Define the viewport transformation
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
    glScalef(window_width/2.0, window_height/2.0, 1.0);
}

void mouse_click (int button, int state, int x, int y)
{
    // GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON
    // GLUT_UP or GLUT_DOWN
    
    float x_coord = x - window_width / 2.0;
    float y_coord = -y  + window_height / 2.0;
    
    // See if any particle was hit
    int hit_particle_id = -1;
    for (int i = 0; i < particles_number; i++)
    {
        Vector2d p_pos = particles[i].position;
        if (abs_d(x_coord - p_pos.x) < min_click_dist && abs_d(y_coord - p_pos.y) < min_click_dist && button == GLUT_LEFT_BUTTON)
        {
            hit_particle_id = particles[i].id;
            break;
        }
    }
    
    if (state == GLUT_UP)
    {
        if (selected_particle_id != -1)
        {
            particles[selected_particle_id].external_acceleration = Vector2d(0.0, 0.0);
            std::cout << "Unselected" << std::endl;
        }
        
        if (hit_particle_id == -1)
            selected_particle_id = -1;
    }
    
    else if (state == GLUT_DOWN)
    {
        
        // Create the first particle
        if (particles_number == 0)
        {
            if (button == GLUT_RIGHT_BUTTON)
                particles.push_back(Particle::create(x_coord, y_coord, true));
            else
                particles.push_back(Particle::create(x_coord, y_coord, false));
        }
        // Particles already exist
        else
        {
            // If a particle was clicked and one was already selected
            if (hit_particle_id != -1 && selected_particle_id != -1)
            {
                Bar new_b = Bar::create(hit_particle_id, selected_particle_id);
                bars.push_back(new_b);
                selected_particle_id = -1; // Unselect
            }
            
            // If a particle was clicked and no particle was selected
            else if (hit_particle_id != -1 && selected_particle_id == -1)
            {
                selected_particle_id = hit_particle_id;
            }
            
            // If a particle was selected but no particle clicked
            else if (hit_particle_id == -1 && selected_particle_id != -1)
            {
                // Create a new particle
                if (button == GLUT_RIGHT_BUTTON)
                    particles.push_back(Particle::create(x_coord, y_coord, true));
                else
                    particles.push_back(Particle::create(x_coord, y_coord, false));
                
                // Create a new bar
                Bar new_b = Bar::create(selected_particle_id, particles_number-1);
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
        selected_particle_id = -1;
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
    else if (key == 'a')
    {
        accelerations = !accelerations;
    }
    else if (key == 'v')
    {
        velocities = !velocities;
    }
    else if (key == 'l')
    {
        lengths = !lengths;
    }
    else if (key == 'e')
    {
        extension_rates = !extension_rates;
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
    float x_coord = x - window_width / 2.0;
    float y_coord = -y  + window_height / 2.0;
    
    for (int i = 0; i < particles_number; i++)
    {
        Vector2d p_pos = particles[i].position;
        if (abs_d(x_coord - p_pos.x) < min_click_dist && abs_d(y_coord - p_pos.y) < min_click_dist)
        {
            particles[i].highlight = true;
        }
        else
            particles[i].highlight = false;
    }
}

void mouse_drag(int x, int y)
{
    float x_coord = x - window_width / 2.0;
    float y_coord = -y  + window_height / 2.0;
    
    if (selected_particle_id != -1)
    {
        Vector2d pos = particles[selected_particle_id].position;
        particles[selected_particle_id].external_acceleration = 5 * Vector2d(x_coord - pos.x, y_coord - pos.y);
    }
}

void draw_particle(const Particle& p)
{
    Vector2d pos = p.position;
    if (p.highlight || selected_particle_id == p.id)
    {
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2f(pos.x*2.0/window_width, pos.y*2.0/window_height);
        glEnd();
    }
    else
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex2f(pos.x*2.0/window_width, pos.y*2.0/window_height);
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
    glVertex2f(start.x*2.0/window_width, start.y*2.0/window_height);
    glVertex2f((start.x + v.x)*2.0/window_width, (start.y + v.y)*2.0/window_height);
    glEnd();
    
    glPointSize(5);
    
    glBegin(GL_POINTS);
    glVertex2f((start.x + v.x)*2.0/window_width, (start.y + v.y)*2.0/window_height);
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
    glVertex2f(start.x*2.0/window_width, start.y*2.0/window_height);
    glVertex2f(end.x*2.0/window_width, end.y*2.0/window_height);
    glEnd();
    
    std::stringstream s;
    s.precision(3);
    
    if (lengths)
    {
        s << b.length();
        glut_print(mid.x, mid.y, s.str());
    }
    if (extension_rates)
    {
        s.str("");
        s << b.extension_rate();
        glut_print(mid.x, mid.y - 10, s.str());
    }
}