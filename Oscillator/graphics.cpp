//
//  graphics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "graphics.h"

#include <sstream>

#include "save.h"
#include "physics.h"
#include "button.h"

int window_width = 1400;
int window_height = 900;

bool accelerations = false;
bool velocities = false;
bool lengths = false;
bool extension_rates = false;
bool coords = true;
bool ids = false;

int min_click_dist = 8; // pixels

double scale = 150.0; // pixels/metre

int selected_particle_id = -1;

bool snap = true;
bool snapped = false;
Vector2d snapped_point(0.0, 0.0);

extern std::vector<Particle> particles;
extern std::vector<Bar> bars;

void glut_print (float x, float y, std::string s, bool px)
// Prints string at location (x,y) in a bitmap font
{
    unsigned short i;
    
    x = convert_to_gl_coords_x(x);
    y = convert_to_gl_coords_y(y);
    
    if (px)
    {
        x /= scale;
        y /= scale;
    }
    
    glRasterPos2f(x, y);
    for (i = 0; i < s.length(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
}

void display_fps(double dt)
{
    glColor3f(1.0, 1.0, 1.0);
    
    std::ostringstream s;
    s << "fps: " << int(1/dt);
    glut_print(-window_width/2.0+30, -window_height/2.0+20, s.str(), true);
}

void display_energy()
{
    glColor3f(1.0, 1.0, 1.0);
    
    std::ostringstream s;
    s.precision(3);
    s << "Energy: " << energy(particles);
    glut_print(window_width/2.0-60, -window_height/2.0+20, s.str(), true);
}

void draw_gravity_indicator()
{
    glColor3f(1.0, 1.0, 1.0);
    
    std::string s;
    if (gravity)
        s = "Gravity ON";
    else
       s = "Gravity OFF";
    glut_print((-window_width/2.0+20)/scale, (window_height/2.0-20)/scale, s);
}

void display()
{
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Switch to the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    
    if (coords)
        draw_coords();
    
    // Draw the particles
    for (int i = 0; i < particles_number; i++)
    {
        draw_particle(particles[i]);
    }
    
    // Draw the bars
    for (int i = 0; i < bars_number; i++)
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
    
    // Draw the walls
    for (int i = 0; i < walls_number; i++)
    {
        draw_wall(walls[i]);
    }
    
    // Draw the snapped point
    if (snap && snapped)
    {
        Vector2d pos_gl(snapped_point.x*2.0*scale/window_width, snapped_point.y*2.0*scale/window_height);
        
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(20);
        glBegin(GL_POINTS);
        glVertex2d(pos_gl.x, pos_gl.y);
        glEnd();
    }

    for (int i = 0; i < buttons_number; i++)
    {
        draw_button(buttons[i]);
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
    
    // Reposition the buttons
    for (int i = 0; i < buttons_number; i++)
    {
        buttons[i].update_position();
    }
}

void mouse_click (int button, int state, int x, int y)
{
    // GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON
    // GLUT_UP or GLUT_DOWN
    
    double x_px, y_px, x_metres, y_metres;
    
    if (snap && snapped)
    {
        x_metres = snapped_point.x;
        y_metres = snapped_point.y;
        x_px = x_metres * scale;
        y_px = y_metres * scale;
    }
    else
    {
        x_px = x - window_width / 2.0;
        y_px = -y  + window_height / 2.0;
        x_metres = x_px / scale;
        y_metres = y_px / scale;
    }
    
    // See if any particle was hit
    int hit_particle_id = -1;
    for (int i = 0; i < particles_number; i++)
    {
        Vector2d p_pos = particles[i].position;
        if (abs_d(x_px - p_pos.x*scale) < min_click_dist && abs_d(y_px - p_pos.y*scale) < min_click_dist && button == GLUT_LEFT_BUTTON)
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
        }
        
        if (hit_particle_id == -1)
            selected_particle_id = -1;
    }
    
    else if (state == GLUT_DOWN)
    {
        bool button_hit = false;
        
        // If a button was hit
        for (int i = 0; i < buttons_number || button_hit == true; i++)
        {
            if (buttons[i].is_hit(x_px, y_px) && state == GLUT_DOWN)
            {
                buttons[i].execute_action();
                std::cout << "Button " << buttons[i].id <<" was clicked" << std::endl;
                button_hit = true;
                return;
            }
        }

        if (button_hit)
        {
            
        }
        
        // If a particle was clicked and one was already selected
        else if (hit_particle_id != -1 && selected_particle_id != -1)
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
                particles.push_back(Particle::create(x_metres, y_metres, true));
            else
                particles.push_back(Particle::create(x_metres, y_metres, false));
            
            // Create a new bar
            Bar new_b = Bar::create(selected_particle_id, particles_number-1);
            bars.push_back(new_b);
            
            selected_particle_id = -1;
        }
        
        // A click on an empty space
        else
        {
            /*if (GLUT_ACTIVE_CTRL)
            {
                walls.push_back(Wall::create(Vector2d(x_metres, y_metres), 1.0, 1.0));
                std::cout << 1 << std::endl;
            }
            
            else*/ if (button == GLUT_RIGHT_BUTTON)
                particles.push_back(Particle::create(x_metres, y_metres, true));
            else
                particles.push_back(Particle::create(x_metres, y_metres, false));
        }
        
        glutPostRedisplay();
    }
}

void key_pressed(unsigned char key, int x, int y)
{
    if (key == 'r')
    {
        reset();
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
    else if (key == 'c')
    {
        coords = !coords;
    }
    else if (key == 's')
    {
        save("/Users/patrick/Desktop/test_save.txt");
    }
    else if (key == 'p')
    {
        load("/Users/patrick/Desktop/cube.txt");
    }
    else if (key == 'i')
    {
        ids = !ids;
    }
    else if (key == 'w')
    {
        walls.push_back(Wall::create(Vector2d(0.0, -0.1), 10, 0.1));
    }
    
    glutPostRedisplay();
}

void idle()
{
    update_time();
    update_position(particles);
    
    glutPostRedisplay();
}

void mouse_passive(int x, int y)
{
    double x_px = x - window_width / 2.0;
    double y_px = -y  + window_height / 2.0;
    double x_metres = x_px / scale;
    double y_metres = y_px / scale;
    
    double closest_x = round(x_metres);
    double closest_y = round(y_metres);
    
    if (abs_d(closest_x*scale-x_px) < min_click_dist && abs_d(closest_y*scale-y_px) < min_click_dist)
    {
        snapped = true;
        snapped_point = Vector2d(closest_x, closest_y);
    }
    else
    {
        snapped = false;
    }
    
    for (int i = 0; i < particles_number; i++)
    {
        Vector2d p_pos = particles[i].position;
        if (abs_d(x_px - p_pos.x*scale) < min_click_dist && abs_d(y_px - p_pos.y*scale) < min_click_dist)
        {
            particles[i].highlight = true;
        }
        else
            particles[i].highlight = false;
    }
    
    if (snap)
    {
        
    }
}

void mouse_drag(int x, int y)
{
    double x_coord = (x - window_width / 2.0)/scale;
    double y_coord = (-y  + window_height / 2.0)/scale;
    
    if (selected_particle_id != -1)
    {
        Vector2d pos = particles[selected_particle_id].position;
        particles[selected_particle_id].external_acceleration = 5 * Vector2d(x_coord - pos.x, y_coord - pos.y);
    }
}

void special_key_down(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
    {
        scale *= 1.1;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        scale /= 1.1;
    }
}

//////////

void draw_particle(const Particle& p)
{
    glColor3f(1.0, 1.0, 1.0);
    
    Vector2d pos = p.position;
    Vector2d pos_gl(pos.x*2.0*scale/window_width, pos.y*2.0*scale/window_height);
    
    if (p.highlight || selected_particle_id == p.id)
    {
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2f(pos_gl.x, pos_gl.y);
        glEnd();
    }
    else
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex2f(pos_gl.x, pos_gl.y);
        glEnd();
    }
    
    if (ids)
    {
        std::stringstream s;
        s << p.id;
        
        Vector2d vel = p.velocity;
        
        glut_print(pos.x - 0.04*vel.norm().x, pos.y - 0.04*vel.norm().y, s.str());
    }
}

void draw_vector(Vector2d v, Vector2d start, float r, float g, float b)
{
    glColor3f(r, g, b);
    glLineWidth(1.0);
    
    glBegin(GL_LINES);
    glVertex2f(start.x*2.0*scale/window_width, start.y*2.0*scale/window_height);
    glVertex2f((start.x + v.x)*2.0*scale/window_width, (start.y + v.y)*2.0*scale/window_height);
    glEnd();
    
    glPointSize(5);
    
    glBegin(GL_POINTS);
    glVertex2f((start.x + v.x)*2.0*scale/window_width, (start.y + v.y)*2.0*scale/window_height);
    glEnd();
    
    glColor3f(1.0, 1.0, 1.0);
}

void draw_bar(const Bar& b)
{
    double epsilon = (b.length() - b.r0)/b.r0;
    if (epsilon > 1.0)
        epsilon = 1.0;
    else if (epsilon < -1.0)
        epsilon = -1.0;
    
    int multiplier = 20;
    
    if (epsilon > 0.0)
        glColor3f(1.0, 1.0-epsilon*multiplier, 1.0-epsilon*multiplier);
    else
        glColor3f(1.0+epsilon*multiplier, 1.0, 1.0);
    
    glLineWidth(1.0);
    
    Vector2d start = particles[b.p1_id].position;
    Vector2d end = particles[b.p2_id].position;
    Vector2d mid(0.5*(start.x + end.x), 0.5*(start.y + end.y));
    
    glBegin(GL_LINES);
    glVertex2f(start.x*2.0*scale/window_width, start.y*2.0*scale/window_height);
    glVertex2f(end.x*2.0*scale/window_width, end.y*2.0*scale/window_height);
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
        glut_print(mid.x, mid.y - 10/scale, s.str());
    }
}

void draw_coords()
{
    glColor3f(0.3, 0.3, 0.3);
    glLineWidth(1.0);
    
    glBegin(GL_LINES);
    
    // Draw the axis
    glVertex2f(0.0, 1.0);
    glVertex2f(0.0, -1.0);
    glVertex2f(-1.0, 0.0);
    glVertex2f(1.0, 0.0);
    
    // Draw the scale
    double scale_size = 5; // px
    
    for (int i = 1; i < window_height/(2.0*scale); i++)
    {
        // For +ve y
        double left_x = -2.0*scale_size/window_width;
        double left_y = i*2.0*scale/window_height;
        double right_x = -left_x;
        double right_y = left_y;
        
        // +ve y
        glVertex2f(left_x, left_y);
        glVertex2f(right_x, right_y);
        
        // -ve y
        glVertex2f(left_x, -left_y);
        glVertex2f(right_x, -right_y);
    }
    
    for (int i = 1; i < window_width/(2.0*scale); i++)
    {
        // For +ve x
        double bottom_x = i*2.0*scale/window_width;
        double bottom_y = -2.0*scale_size/window_height;
        double top_x = bottom_x;
        double top_y = -bottom_y;
        
        // +ve x
        glVertex2f(bottom_x, bottom_y);
        glVertex2f(top_x, top_y);
        
        // -ve x
        glVertex2f(-bottom_x, bottom_y);
        glVertex2f(-top_x, top_y);
    }
    
    glEnd();
}

Vector2d convert_to_gl_coords(const Vector2d& v)
{
    return Vector2d(v.x * 2.0 * scale / window_width, v.y * 2.0 * scale / window_height);
}

double convert_to_gl_coords_x(double d)
{
    return d * 2.0 * scale / window_width;
}

double convert_to_gl_coords_y(double d)
{
    return d * 2.0 * scale / window_height;
}

void draw_rectangle(Vector2d c, double w, double h, bool px)
{
    Vector2d centre;
    double half_width, half_height;
    
    if (px)
    {
        // Convert from pixels to gl coords
        double scale_inverse = 1.0 / scale;
        centre = convert_to_gl_coords(scale_inverse * c);
        half_width = 0.5 * convert_to_gl_coords_x(scale_inverse * w);
        half_height = 0.5 * convert_to_gl_coords_y(scale_inverse * h);
    }
    else
    {
        // Convert from metres to gl coords
        centre = convert_to_gl_coords(c);
        half_width = 0.5 * convert_to_gl_coords_x(w);
        half_height = 0.5 * convert_to_gl_coords_y(h);
    }
    
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(1.0);
    
    glBegin(GL_LINE_LOOP);
    glVertex2f(centre.x - half_width, centre.y + half_height);
    glVertex2f(centre.x + half_width, centre.y + half_height);
    glVertex2f(centre.x + half_width, centre.y - half_height);
    glVertex2f(centre.x - half_width, centre.y - half_height);
    glEnd();
}

void draw_button(const Button& rect)
{
    draw_rectangle(rect.centre_, rect.width_, rect.height_, true);
}

void draw_wall(const Wall& w)
{
    draw_rectangle(w.centre_, w.width_, w.height_, false);
}