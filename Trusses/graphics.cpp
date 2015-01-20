//
//  graphics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "graphics.h"

#include <sstream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

#include "save.h"
#include "physics.h"
#include "button.h"
#include "interface.h"
#include "interpreter.h"

int window_width = 1200;
int window_height = 700;

bool accelerations = false;
bool velocities = false;
bool lengths = false;
bool extensions = false;
bool coords = true;
bool ids = false;

extern std::vector<Particle> particles; // TODO: remove these?
extern std::vector<Bar> bars;

void glut_print (float x, float y, std::string s, bool px)
// Prints string at location (x,y) in a bitmap font
{
    unsigned short i;
    
    if (px)
    {
        x = px_to_gl_coords_x(x);
        y = px_to_gl_coords_y(y);
    }
    else
    {
        x = metres_to_gl_coords_x(x);
        y = metres_to_gl_coords_y(y);
    }
    
    glRasterPos2f(x, y);
    for (i = 0; i < s.length(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[i]);
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
    s.precision(5);
    s << "Energy: " << energy(particles);
    glut_print(window_width/2.0-120, -window_height/2.0+20, s.str(), true);
}

void draw_gravity_indicator()
{
    glColor3f(1.0, 1.0, 1.0);
    
    std::string s;
    if (gravity)
        s = "Gravity ON";
    else
       s = "Gravity OFF";
    glut_print(-window_width/2.0+20, window_height/2.0-20, s, true);
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
    
    // Draw the walls
    for (int i = 0; i < walls_number; i++)
    {
        draw_wall(walls[i]);
    }
    
    // Draw the bars
    for (int i = 0; i < bars_number; i++)
    {
        draw_bar(bars[i]);
    }
    
    // Draw the particles
    for (int i = 0; i < particles_number; i++)
    {
        draw_particle(particles[i]);
    }
    
    // Draw the vectors
    for (int i = 0; i < particles_number; i++)
    {
        if (!particles[i].fixed_)
        {
            // Draw the velocity vector
            if (velocities)
                draw_vector(particles[i].velocity_, particles[i].position_, 0.0, 0.5, 0.0);
            
            // Draw the acceleration vector
            if (accelerations)
                draw_vector(particles[i].acceleration_, particles[i].position_, 0.5, 0.0, 0.0);
        }
    }
    
    // Draw the snapped point
    if (snap && snapped)
    {
        Vector2d pos_gl(metres_to_gl_coords_x(snapped_point.x), metres_to_gl_coords_y(snapped_point.y));
        
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2d(pos_gl.x, pos_gl.y);
        glEnd();
    }

    // Draw buttons
    for (int i = 0; i < buttons_number; i++)
    {
        draw_button(buttons[i]);
    }
    
    display_fps(delta_t);
    display_energy();
    draw_gravity_indicator();
    
    // Draw the command line
    if (command_mode)
    {
        glColor3f(0.3, 0.3, 0.3);
        glBegin(GL_QUADS);
        glVertex2f(px_to_gl_coords_x(-window_width/2.0), px_to_gl_coords_y(-window_height/2.0));
        glVertex2f(px_to_gl_coords_x(window_width/2.0), px_to_gl_coords_y(-window_height/2.0));
        glVertex2f(px_to_gl_coords_x(window_width/2.0), px_to_gl_coords_y(-window_height/2.0+30));
        glVertex2f(px_to_gl_coords_x(-window_width/2.0), px_to_gl_coords_y(-window_height/2.0+30));
        glEnd();
        
        glColor3f(0.7, 0.7, 0.0);
        glBegin(GL_LINES);
        glVertex2f(px_to_gl_coords_x(window_width/2.0), px_to_gl_coords_y(-window_height/2.0+30));
        glVertex2f(px_to_gl_coords_x(-window_width/2.0), px_to_gl_coords_y(-window_height/2.0+30));
        glEnd();
        
        glColor3f(1.0, 1.0, 1.0);
        glut_print(-window_width/2.0+20, -window_height/2.0+10, command, true);
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
    glScalef(window_width/2.0, window_height/2.0, 1.0);
    
    // Reposition the buttons
    for (int i = 0; i < buttons_number; i++)
    {
        buttons[i].update_position();
    }
}

void idle()
{
    update_time();
    update_position(particles);
    
    glutPostRedisplay();
}

//////////

void draw_particle(const Particle& p)
{
    Vector2d pos = p.position_;
    Vector2d pos_gl = metres_to_gl_coords(pos);
    
    // If selected
    if (selected_particle_id == p.id_.number)
    {
        glColor3f(1.0, 1.0, 0.0);
        glPointSize(10);
    }
    // If fixed
    else if (p.fixed_)
    {
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(8);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(5);
    }
    
    // Increase the point size if highlighted and not selected
    if (highlighted_particle_id == p.id_.number && selected_particle_id != p.id_.number)
    {
        glPointSize(10);
    }
    
    glBegin(GL_POINTS);
    glVertex2f(pos_gl.x, pos_gl.y);
    glEnd();
    
    if (ids)
    {
        std::stringstream s;
        s << p.id_.number;
        
        // Add 5 pixels in eah direction
        glColor3f(0.8, 0.8, 0.0);
        glut_print(pos.x + px_to_m(5.0), pos.y + px_to_m(5.0), s.str());
    }
}

void draw_vector(Vector2d v, Vector2d start, float r, float g, float b)
{
    Vector2d gl_start = metres_to_gl_coords(start);
    Vector2d gl_end = metres_to_gl_coords(start + v);
    
    glColor3f(r, g, b);
    glLineWidth(1.0);
    
    glBegin(GL_LINES);
    glVertex2f(gl_start.x, gl_start.y);
    glVertex2f(gl_end.x, gl_end.y);
    glEnd();
    
    glPointSize(5);
    
    glBegin(GL_POINTS);
    glVertex2f(gl_end.x, gl_end.y);
    glEnd();
}

void draw_bar(const Bar& b)
{
    // Relative extension
    double epsilon = b.extension() / b.length();
    if (epsilon > 1.0)
        epsilon = 1.0;
    else if (epsilon < -1.0)
        epsilon = -1.0;
    
    int multiplier = 20;
    
    if (epsilon > 0.0)
        glColor3f(1.0, 1.0-epsilon*multiplier, 1.0-epsilon*multiplier);
    else
        glColor3f(1.0+epsilon*multiplier, 1.0, 1.0);
    
    glLineWidth(2.0);
    
    Vector2d gl_start = metres_to_gl_coords(particles[b.p1_id].position_);
    Vector2d gl_end = metres_to_gl_coords(particles[b.p2_id].position_);
    
    Vector2d m_mid = 0.5 * (particles[b.p1_id].position_ + particles[b.p2_id].position_);
    
    glBegin(GL_LINES);
    glVertex2f(gl_start.x, gl_start.y);
    glVertex2f(gl_end.x, gl_end.y);
    glEnd();
    
    std::stringstream s;
    s.precision(3);
    
    if (lengths)
    {
        s << b.length();
        glut_print(m_mid.x, m_mid.y, s.str());
    }
    if (extensions)
    {
        s.str("");
        s << b.extension() / b.r0;
        glut_print(m_mid.x, m_mid.y - px_to_m(12.0), s.str());
    }
}

void draw_coords()
{
    // Find the position of the centre in gl coords
    Vector2d gl_centre = px_to_gl_coords(world_centre);
    
    // Draw the axis
    glColor3f(0.3, 0.3, 0.3);
    glLineWidth(2.0);
    
    glBegin(GL_LINES);
    glVertex2f(gl_centre.x, 1.0);
    glVertex2f(gl_centre.x, -1.0);
    glVertex2f(-1.0, gl_centre.y);
    glVertex2f(1.0, gl_centre.y);
    glEnd();
    
    // Draw the scale
    glColor3f(0.3, 0.3, 0.3);
    glLineWidth(1.0);
    
    glBegin(GL_LINES);
    
    double m_dist = 1.0; // Distance between lines in metres
    
    // For +ve y
    for (int i = 1; metres_to_gl_coords_y(i * m_dist) < 1.0; i++)
    {
        double y_pos = metres_to_gl_coords_y(i * m_dist);
        glVertex2f(-1.0, y_pos);
        glVertex2f(1.0, y_pos);
    }
    // For -ve y
    for (int i = -1; metres_to_gl_coords_y(i * m_dist) > -1.0; i--)
    {
        double y_pos = metres_to_gl_coords_y(i * m_dist);
        glVertex2f(-1.0, y_pos);
        glVertex2f(1.0, y_pos);
    }
    // For +ve x
    for (int i = 1; metres_to_gl_coords_x(i * m_dist) < 1.0; i++)
    {
        double x_pos = metres_to_gl_coords_x(i * m_dist);
        glVertex2f(x_pos, -1.0);
        glVertex2f(x_pos, 1.0);
    }
    // For -ve x
    for (int i = -1; metres_to_gl_coords_x(i * m_dist) > -1.0; i--)
    {
        double x_pos = metres_to_gl_coords_x(i * m_dist);
        glVertex2f(x_pos, -1.0);
        glVertex2f(x_pos, 1.0);
    }
    
    glEnd();
    
    // Draw the scale (as a number)
    std::ostringstream s;
    s << m_dist;
    glColor3f(0.5, 0.5, 0.5);
    glut_print(m_dist, 0.0, s.str() + "m");
}

/////

// Converts the internal world coordinates to screen gl coordinates (between -1.0 and 1.0)
Vector2d metres_to_gl_coords(const Vector2d& v)
{
    return Vector2d(metres_to_gl_coords_x(v.x), metres_to_gl_coords_y(v.y));
}

double metres_to_gl_coords_x(double d)
{
    return (d + world_centre.x / scale) * 2.0 * scale / window_width;
}

double metres_to_gl_coords_y(double d)
{
    return (d + world_centre.y / scale) * 2.0 * scale / window_height;
}

Vector2d px_to_gl_coords(Vector2d v)
{
    return Vector2d(px_to_gl_coords_x(v.x), px_to_gl_coords_y(v.y));
}

double px_to_gl_coords_x(double d)
{
    return d * 2.0 / window_width;
}

double px_to_gl_coords_y(double d)
{
    return d * 2.0 / window_height;
}

/////

void draw_rectangle(Vector2d p1, Vector2d p2, bool px)
{
    
    Vector2d p1_gl;
    Vector2d p2_gl;
    
    if (px)
    {
        // Convert from pixels to gl coords
        p1_gl = px_to_gl_coords(p1);
        p2_gl = px_to_gl_coords(p2);
    }
    else
    {
        // Convert from metres to gl coords
        p1_gl = metres_to_gl_coords(p1);
        p2_gl = metres_to_gl_coords(p2);
    }
    
    glBegin(GL_LINE_LOOP);
    glVertex2f(p1_gl.x, p1_gl.y);
    glVertex2f(p1_gl.x, p2_gl.y);
    glVertex2f(p2_gl.x, p2_gl.y);
    glVertex2f(p2_gl.x, p1_gl.y);
    glEnd();
}

void draw_button(const Button& b)
{
    if (b.active_)
    {
        glColor3f(1.0, 1.0, 0.0);
        glLineWidth(2.0);
    }
    else if (b.highlighted_)
    {
        glColor3f(0.8, 0.8, 0.0);
        glLineWidth(2.0);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(2.0);
    }
    
    Vector2d p1(b.centre_.x - b.width_/2.0, b.centre_.y - b.height_/2.0);
    Vector2d p2(b.centre_.x + b.width_/2.0, b.centre_.y + b.height_/2.0);
    
    draw_rectangle(p1, p2, true);
    
    glut_print(b.centre_.x - b.width_/2.0 + 5, b.centre_.y - 4, b.text_, true);
}

void draw_wall(const Wall& w)
{
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    
    draw_rectangle(w.p1_, w.p2_, false);
    
    double x_min = w.x_min();
    double x_max = w.x_max();
    double y_min = w.y_min();
    double y_max = w.y_max();
    
    double width = x_max - x_min;
    double height = y_max - y_min;
    
    // Spacing between the lines
    double d = 0.25;
    
    glBegin(GL_LINES);
    for (int i = 0; i * d <= width + height; i++)
    {
        double x1 = x_min + i * d - height;
        double y1 = y_min;
        double x2 = x_min + i * d;
        double y2 = y_max;
        
        if (x1 < x_min)
        {
            x1 = x_min;
            y1 = y_max - i * d;
        }
        if (x2 > x_max)
        {
            x2 = x_max;
            y2 = y_min + width + height - i * d;
        }
        
        glVertex2f(metres_to_gl_coords_x(x1), metres_to_gl_coords_y(y1));
        glVertex2f(metres_to_gl_coords_x(x2), metres_to_gl_coords_y(y2));
        
    }
    glEnd();
    
    // Print ids
    std::ostringstream s;
    s << '(' << w.id_.number << ", " << w.id_.version << ')';
    glColor3f(0.0, 1.0, 1.0);
    glut_print(w.p2_.x, w.p2_.y, s.str());
}