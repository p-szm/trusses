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

#include "particle.h"
#include "bar.h"
#include "wall.h"
#include "button.h"
#include "physics.h"
#include "interface.h"
#include "interpreter.h"
#include "temporary_label.h"

// * * * * * * * * * * //
// Forward declarations

enum coord_t {PX, WORLD, GL};
void glut_print (float x, float y, std::string s, coord_t coord_type);
void display_fps(double dt);
void display_time();
void display_temperature(double temp);
void draw_particle(Particle& p);
void draw_bar(const Bar& b);
void draw_wall(const Wall& w);
void draw_vector(Vector2d v, Vector2d start, float r, float g, float b);
void draw_coords();
void draw_command_line();
void draw_button(const Button& b);
void draw_rectangle(Vector2d p1, Vector2d p2, coord_t coord_type, bool filled);
void draw_circle(Vector2d centre, double r, unsigned int n_points);
void draw_cross(Vector2d pos, int size_px);

// * * * * * * * * * * //
inline Vector2d world_to_gl_coords(const Vector2d& v);
inline double world_to_gl_coords_x(double d);
inline double world_to_gl_coords_y(double d);

// * * * * * * * * * * //
vec3 hsv_to_rgb(vec3 hsv);

// * * * * * * * * * * //
int window_width = 1200;
int window_height = 800;
bool accelerations = false;
bool velocities = false;
bool lengths = false;
bool extensions = false;
bool coords = true;
bool ids = false;
bool fancy_bars = false;
bool show_particles = true;
bars_color_mode_t bars_color_mode = STRAIN_C;

// * * * * * * * * * * //
//
// Converts the internal world coordinates to screen gl coordinates (between -1.0 and 1.0)
Vector2d world_to_gl_coords(const Vector2d& v)
{ return Vector2d(world_to_gl_coords_x(v.x), world_to_gl_coords_y(v.y)); }

double world_to_gl_coords_x(double d)
{ return (d + world_centre.x / scale) * 2.0 * scale / window_width; }

double world_to_gl_coords_y(double d)
{ return (d + world_centre.y / scale) * 2.0 * scale / window_height; }

Vector2d px_to_gl(const Vector2d& v)
{ return Vector2d(px_to_gl_x(v.x), px_to_gl_y(v.y)); }

double px_to_gl_x(double d)
{ return d * 2.0 / window_width; }

double px_to_gl_y(double d)
{ return d * 2.0 / window_height; }

// * * * * * * * * * * //
void glut_print (float x, float y, std::string s, coord_t coord_type)
// Prints string at location (x,y) in a bitmap font
{
    if (coord_type == PX)
    {
        x = px_to_gl_x(x);
        y = px_to_gl_y(y);
    }
    else if (coord_type == WORLD)
    {
        x = world_to_gl_coords_x(x);
        y = world_to_gl_coords_y(y);
    }
    glRasterPos2f(x, y);
    for (unsigned short i = 0; i < s.length(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[i]);
}

void display_fps(double dt)
{
    glColor3f(1.0, 1.0, 1.0);
    
    std::ostringstream s;
    s << "fps: " << int(1/dt);
    glut_print(-window_width/2.0+30, -window_height/2.0+20, s.str(), PX);
}

void display_time()
{
    glColor3f(1.0, 1.0, 1.0);
    
    std::ostringstream s;
    s.precision(1);
    s << "Time: " << std::fixed << simulation_time/1000000.0 << " s";
    glut_print(0, -window_height/2.0+20, s.str(), PX);
}

void display_temperature(double temp)
{
    if (temp > MELTING_POINT)
        glColor3f(0.8, 0.0, 0.0);
    else
        glColor3f(1.0, 1.0, 1.0);
    
    std::ostringstream s;
    s.precision(5);
    s << "T = " << int(temp) << " K";
    glut_print(window_width/2.0-80, -window_height/2.0+20, s.str(), PX);
}

void draw_particle(Particle& p)
{
    // Draw the trace if it is enabled
    glColor3f(0.8, 0.8, 0.0);
    glPointSize(1);
    glBegin(GL_POINTS);
    for (int i = 0; i < p.trace.size(); i++)
    {
        Vector2d t = world_to_gl_coords(p.trace.get(i));
        glVertex2f(t.x, t.y);
    }
    glEnd();
    
    // Particle's position
    Vector2d pos = p.position_;
    Vector2d pos_gl = world_to_gl_coords(pos);
    
    // If fixed
    if (p.fixed_)
    {
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(8);
        glBegin(GL_POINTS);
        glVertex2f(pos_gl.x, pos_gl.y);
        glEnd();
    }
    else if (show_particles)
    {
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(6);
        glBegin(GL_POINTS);
        glVertex2f(pos_gl.x, pos_gl.y);
        glEnd();
    }
    
    if (ids)
    {
        std::stringstream s;
        s << p.id_;
        
        // Add 5 pixels in eah direction
        glColor3f(0.8, 0.8, 0.0);
        glut_print(pos.x + px_to_m(5.0), pos.y + px_to_m(5.0), s.str(), WORLD);
    }
}

void draw_bar(const Bar& b)
{
    // Color bars according to their strain
    // TODO: Scale it appropriately (use MAX_STRAIN)
    if (bars_color_mode == STRAIN_C)
    {
        // Relative extension
        double strain = b.get_strain();
        if (strain > 1.0)
            strain = 1.0;
        else if (strain < -1.0)
            strain = -1.0;
        
        if (strain > 0.0)
            glColor3f(1.0, 1.0 - strain / MAX_STRAIN, 1.0 - strain / MAX_STRAIN);
        else
            glColor3f(1.0 + strain / MAX_STRAIN, 1.0, 1.0);
    }
    // Color bars according to their temperature
    // TODO: Color it appropriately: black-red-yellow-white
    else if (bars_color_mode == TEMP_C)
    {
        double temp_fraction = (b.get_temperature() - ROOM_TEMPERATURE) / (MELTING_POINT - ROOM_TEMPERATURE);
        if (temp_fraction > 0.0)
            glColor3f(1.0, 1.0 - temp_fraction, 1.0 - temp_fraction); // red
        else
            glColor3f(1.0 + temp_fraction, 1.0, 1.0); // cyan
        
        // TODO
        vec3 color;
        double temp_chunk = MELTING_POINT / 3.0;
        double temp = b.get_temperature();
        if (temp <= temp_chunk)
            color = vec3(temp / temp_chunk, 0, 0);
        else if (temp <= 2 * temp_chunk)
            color = vec3(1.0, (temp - temp_chunk) / temp_chunk, 0.0);
        else if (temp <= 3 * temp_chunk)
            color = vec3(1.0, 1.0, (temp - 2 * temp_chunk) / temp_chunk);
        else
            color = vec3(1.0, 1.0, 1.0);
        glColor3f(color.x, color.y, color.z);
    }
    
    Vector2d gl_start = world_to_gl_coords(particles[b.p1_id].position_);
    Vector2d gl_end = world_to_gl_coords(particles[b.p2_id].position_);
    Vector2d m_mid = 0.5 * (particles[b.p1_id].position_ + particles[b.p2_id].position_);
    
    if (!fancy_bars)
    {
        glLineWidth(2.0);
        
        glBegin(GL_LINES);
        glVertex2f(gl_start.x, gl_start.y);
        glVertex2f(gl_end.x, gl_end.y);
        glEnd();
    }
    
    else
    {
        glLineWidth(1.0);
        
        Vector2d p1_pos = particles[b.p1_id].position_;
        Vector2d p2_pos = particles[b.p2_id].position_;
        
        // Draw circles at the ends
        double b_radius = 0.02;
        draw_circle(p1_pos, b_radius, 20); // 0.2m
        draw_circle(p2_pos, b_radius, 20);
        
        Vector2d unit = (p2_pos - p1_pos).norm();
        Vector2d normal = Vector2d(-unit.y, unit.x);
        
        Vector2d p1 = p1_pos + b_radius * normal;
        Vector2d p2 = p2_pos + b_radius * normal;
        Vector2d p3 = p1_pos - b_radius * normal;
        Vector2d p4 = p2_pos - b_radius * normal;
        
        glBegin(GL_LINES);
        glVertex2f(world_to_gl_coords_x(p1.x), world_to_gl_coords_y(p1.y));
        glVertex2f(world_to_gl_coords_x(p2.x), world_to_gl_coords_y(p2.y));
        glVertex2f(world_to_gl_coords_x(p3.x), world_to_gl_coords_y(p3.y));
        glVertex2f(world_to_gl_coords_x(p4.x), world_to_gl_coords_y(p4.y));
        glEnd();
    }
    
    std::stringstream s;
    s.precision(3);
    
    if (ids)
    {
        glColor3f(0.8, 0.0, 0.8);
        s << b.id_;
        glut_print(m_mid.x, m_mid.y, s.str(), WORLD);
    }
    if (lengths)
    {
        s << b.length();
        glut_print(m_mid.x, m_mid.y, s.str(), WORLD);
    }
    if (extensions)
    {
        s.str("");
        s << b.get_strain();
        glut_print(m_mid.x, m_mid.y - px_to_m(12.0), s.str(), WORLD);
    }
}

void draw_wall(const Wall& w)
{
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(2.0);
    
    draw_rectangle(w.p1_, w.p2_, WORLD, false);
    
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
        
        glVertex2f(world_to_gl_coords_x(x1), world_to_gl_coords_y(y1));
        glVertex2f(world_to_gl_coords_x(x2), world_to_gl_coords_y(y2));
        
    }
    glEnd();
    
    if (ids)
    {
        std::ostringstream s;
        s << w.id_;
        glColor3f(0.0, 0.8, 0.8);
        glut_print(w.p2_.x, w.p2_.y, s.str(), WORLD);
    }
}

void draw_vector(Vector2d v, Vector2d start, float r, float g, float b)
{
    Vector2d gl_start = world_to_gl_coords(start);
    Vector2d gl_end = world_to_gl_coords(start + v);
    
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

void draw_coords()
{
    // Find the position of the centre in gl coords
    Vector2d gl_centre = px_to_gl(world_centre);
    
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
    
    double m_dist = grid_dist_px / scale; // Distance between lines in metres
    
    // For +ve y
    for (int i = 1; world_to_gl_coords_y(i * m_dist) < 1.0; i++)
    {
        double y_pos = world_to_gl_coords_y(i * m_dist);
        glVertex2f(-1.0, y_pos);
        glVertex2f(1.0, y_pos);
    }
    // For -ve y
    for (int i = -1; world_to_gl_coords_y(i * m_dist) > -1.0; i--)
    {
        double y_pos = world_to_gl_coords_y(i * m_dist);
        glVertex2f(-1.0, y_pos);
        glVertex2f(1.0, y_pos);
    }
    // For +ve x
    for (int i = 1; world_to_gl_coords_x(i * m_dist) < 1.0; i++)
    {
        double x_pos = world_to_gl_coords_x(i * m_dist);
        glVertex2f(x_pos, -1.0);
        glVertex2f(x_pos, 1.0);
    }
    // For -ve x
    for (int i = -1; world_to_gl_coords_x(i * m_dist) > -1.0; i--)
    {
        double x_pos = world_to_gl_coords_x(i * m_dist);
        glVertex2f(x_pos, -1.0);
        glVertex2f(x_pos, 1.0);
    }
    
    glEnd();
    
    // Choose the right units to display
    double si_dist = 0.0;
    std::string unit;
    
    if (m_dist < 1e-3)
    {
        si_dist = m_dist * 1e6;
        unit = "um";
    }
    else if (m_dist < 1e-2)
    {
        si_dist = m_dist * 1e3;
        unit = "mm";
    }
    else if (m_dist < 1.0)
    {
        si_dist = m_dist * 1e2;
        unit = "cm";
    }
    else
    {
        si_dist = m_dist;
        unit = "m";
    }
    
    // Draw the scale (as a number)
    std::ostringstream s;
    s.precision(2);
    s << si_dist;
    glColor3f(0.5, 0.5, 0.5);
    glut_print(m_dist, 0.0, s.str() + unit, WORLD);
}

void draw_command_line()
{
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(px_to_gl_x(-window_width/2.0), px_to_gl_y(-window_height/2.0));
    glVertex2f(px_to_gl_x(window_width/2.0), px_to_gl_y(-window_height/2.0));
    glVertex2f(px_to_gl_x(window_width/2.0), px_to_gl_y(-window_height/2.0 + COMMAND_LINE_SIZE));
    glVertex2f(px_to_gl_x(-window_width/2.0), px_to_gl_y(-window_height/2.0 + COMMAND_LINE_SIZE));
    glEnd();
    
    glColor3f(0.7, 0.7, 0.0);
    glBegin(GL_LINES);
    glVertex2f(px_to_gl_x(window_width/2.0), px_to_gl_y(-window_height/2.0 + COMMAND_LINE_SIZE));
    glVertex2f(px_to_gl_x(-window_width/2.0), px_to_gl_y(-window_height/2.0 + COMMAND_LINE_SIZE));
    glEnd();
    
    glColor3f(1.0, 1.0, 1.0);
    glut_print(-window_width/2.0+20, -window_height/2.0+10, commands[commands.size() - current_cmd - 1], PX);
}

void draw_button(const Button& b)
{
    Vector2d centre_gl = b.position + px_to_gl(b.offset);
    Vector2d size_gl = px_to_gl(Vector2d(b.width_/2.0, b.height_/2.0));
    glColor3f(0.2, 0.2, 0.2);
    draw_rectangle(centre_gl - size_gl, centre_gl + size_gl, GL, true);
    
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
    draw_rectangle(centre_gl - size_gl, centre_gl + size_gl, GL, false);
    
    glut_print(b.position.x + px_to_gl_x(b.offset.x - b.width_/2.0 + 6), b.position.y + px_to_gl_y(b.offset.y - 5), b.text_, GL);
}

void draw_rectangle(Vector2d p1, Vector2d p2, coord_t coord_type, bool filled)
{
    Vector2d p1_gl, p2_gl;
    if (coord_type == PX)
    {
        // Convert from pixels to gl coords
        p1_gl = px_to_gl(p1);
        p2_gl = px_to_gl(p2);
    }
    else if (coord_type == WORLD)
    {
        // Convert from metres to gl coords
        p1_gl = world_to_gl_coords(p1);
        p2_gl = world_to_gl_coords(p2);
    }
    else if (coord_type == GL)
    {
        p1_gl = p1;
        p2_gl = p2;
    }
    
    if (!filled)
    {
        glBegin(GL_LINE_LOOP);
        glVertex2f(p1_gl.x, p1_gl.y);
        glVertex2f(p1_gl.x, p2_gl.y);
        glVertex2f(p2_gl.x, p2_gl.y);
        glVertex2f(p2_gl.x, p1_gl.y);
        glEnd();
    }
    else
    {
        glBegin(GL_QUADS);
        glVertex2f(p1_gl.x, p1_gl.y);
        glVertex2f(p1_gl.x, p2_gl.y);
        glVertex2f(p2_gl.x, p2_gl.y);
        glVertex2f(p2_gl.x, p1_gl.y);
        glEnd();
    }
}

// All in metres
void draw_circle(Vector2d centre, double r, unsigned int n_points)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < n_points; i++)
    {
        glVertex2f(world_to_gl_coords_x(centre.x + r * cos(i * 2 * M_PI / n_points)), world_to_gl_coords_y(centre.y + r * sin(i * 2 * M_PI / n_points)));
    }
    glEnd();
}

// Position in world coords, size in px
void draw_cross(Vector2d pos, int size_px)
{
    Vector2d pos_gl = world_to_gl_coords(pos);
    
    glBegin(GL_LINES);
    glVertex2f(pos_gl.x - px_to_gl_x(size_px), pos_gl.y);
    glVertex2f(pos_gl.x + px_to_gl_x(size_px), pos_gl.y);
    glVertex2f(pos_gl.x, pos_gl.y + px_to_gl_y(size_px));
    glVertex2f(pos_gl.x , pos_gl.y - px_to_gl_y(size_px));
    glEnd();
}

// * * * * * * * * * * //
vec3 hsv_to_rgb(vec3 hsv) // H is in the range [0,360] degs
{
    // TODO: check if given hsv is valid
    
    double C = hsv.z * hsv.y;
    double B = fmod((hsv.x/60.0), 2.0) - 1;
    double X = C * (1 - abs_d(B));
    double m = hsv.z - C;
    int n = (int)hsv.x / 60;
    
    vec3 rgb;
    if (n == 0)
        rgb = vec3(C, X, 0);
    else if (n == 1)
        rgb = vec3(X, C, 0);
    else if (n == 2)
        rgb = vec3(0, C, X);
    else if (n == 3)
        rgb = vec3(0, X, C);
    else if (n == 4)
        rgb = vec3(X, 0, C);
    else if (n == 5)
        rgb = vec3(C, 0, X);
    
    rgb.x += m;
    rgb.y += m;
    rgb.z += m;
    return rgb;
}

// * * * * * * * * * * //
void display()
{
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    bool drawing_mode = simulation_is_paused();
    
    if (drawing_mode && coords)
        draw_coords();
    
    // Draw the walls
    SlotMap<Wall>::iterator walls_it;
    for (walls_it = walls.begin(); walls_it != walls.end(); walls_it++)
        draw_wall(*walls_it);
    
    // Draw the bars
    SlotMap<Bar>::iterator bars_it;
    for (bars_it = bars.begin(); bars_it != bars.end(); bars_it++)
        draw_bar(*bars_it);
    
    // Draw the particles
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
        draw_particle(*particles_it);
    
    // Draw the vectors
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
    {
        if (!particles_it->fixed_)
        {
            // Draw the velocity vector
            if (velocities)
                draw_vector(particles_it->velocity_, particles_it->position_, 0.0, 0.5, 0.0);
            
            // Draw the acceleration vector
            if (accelerations)
                draw_vector(particles_it->acceleration_, particles_it->position_, 0.5, 0.0, 0.0);
        }
    }
    
    // Only applies to the drawing mode
    if (drawing_mode)
    {
        // Draw the wall points
        for (int i = 0; i < wall_points.size(); i++)
        {
            glLineWidth(1);
            glColor3f(1.0, 1.0, 1.0);
            draw_cross(wall_points[i], 10);
        }
        
        // Draw the wall cursor
        if (drawing_wall)
        {
            glLineWidth(1);
            glColor3f(1.0, 1.0, 1.0);
            draw_cross(mouse_pos, 10);
        }
        
        // Draw dashed lines between selected particles and mouse
        // Draw the highlight for each selected particle
        if (selected_particles_ids.size() != 0)
        {
            // Dashed lines
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x00FF);
            glLineWidth(1.0);
            glColor3f(1.0, 1.0, 0.0);
            
            glBegin(GL_LINES);
            for (int i = 0; i < selected_particles_ids.size(); i++)
            {
                if (particles.exists(selected_particles_ids[i])) // Make sure that this particle exists
                {
                    Vector2d selected_pos = particles[selected_particles_ids[i]].position_;
                    glVertex2f(world_to_gl_coords_x(selected_pos.x), world_to_gl_coords_y(selected_pos.y));
                    glVertex2f(world_to_gl_coords_x(mouse_pos.x), world_to_gl_coords_y(mouse_pos.y));
                }
            }
            glEnd();
            
            // Highlights
            glColor3f(1.0, 1.0, 1.0);
            glPointSize(8);
            
            glBegin(GL_POINTS);
            for (int i = 0; i < selected_particles_ids.size(); i++)
            {
                int selected_id = selected_particles_ids[i];
                if (particles.exists(selected_id)) // Make sure that this particle exists
                {
                    Vector2d pos_gl = world_to_gl_coords(particles[selected_id].position_);
                    glVertex2f(pos_gl.x, pos_gl.y);
                }
            }
            glEnd();
            
            glDisable(GL_LINE_STIPPLE);
        }
    }
    
    // Draw the snapped point
    if (snap && snapped && !drawing_wall)
    {
        Vector2d pos_gl(world_to_gl_coords_x(mouse_pos.x), world_to_gl_coords_y(mouse_pos.y));
        
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2f(pos_gl.x, pos_gl.y);
        glEnd();
    }
    
    // Draw the active particle
    if (!drawing_mode && particles.exists(active_particle_id))
    {
        Particle* active_p = &particles[active_particle_id];
        Vector2d particle_pos_gl = world_to_gl_coords( active_p->position_ );
        Vector2d mouse_pos_gl = world_to_gl_coords(mouse_pos);
        
        glColor3f(0.8, 0.8, 0.0);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2f(particle_pos_gl.x, particle_pos_gl.y);
        glEnd();
        
        glLineWidth(1.0);
        glBegin(GL_LINES);
        glVertex2f(particle_pos_gl.x, particle_pos_gl.y);
        glVertex2f(mouse_pos_gl.x, mouse_pos_gl.y);
        glEnd();
    }
    
    // Draw temporary labels
    SlotMap<TempLabel>::iterator labels_it;
    for (labels_it = temp_labels.begin(); labels_it != temp_labels.end(); labels_it++)
    {
        glColor4f(1.0, 1.0, 1.0, labels_it->alpha());
        if (labels_it->centre)
            glut_print(labels_it->position.x + px_to_gl_x(labels_it->offset.x - labels_it->text.size() * 2.8),
                   labels_it->position.y + px_to_gl_y(labels_it->offset.y - 6),
                   labels_it->text, GL);
        else
            glut_print(labels_it->position.x + px_to_gl_x(labels_it->offset.x),
                       labels_it->position.y + px_to_gl_y(labels_it->offset.y),
                       labels_it->text, GL);
    }

    // Draw buttons
    for (int i = 0; i < buttons_number; i++)
        draw_button(buttons[i]);
    
    //display_fps(delta_t);
    display_temperature(environment_temp);
    display_time();
    
    // Draw the command line
    if (command_mode)
        draw_command_line();
    
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    // Define the viewport transformation
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
    glScalef(window_width/2.0, window_height/2.0, 1.0);
}

void idle()
{
    update_time();
    
    // Update labels
    std::vector<int> labels_to_remove;
    SlotMap<TempLabel>::iterator labels_it;
    for (labels_it = temp_labels.begin(); labels_it != temp_labels.end(); labels_it++)
        if (TempLabel::update(labels_it->id_))
            labels_to_remove.push_back(labels_it->id_);
    
    // Remove labels that expired
    for (int i = 0; i < labels_to_remove.size(); i++)
        TempLabel::destroy(labels_to_remove[i]);
    
    if (!simulation_is_paused())
    {
        // TODO: This is pretty accurate, but could be better
        // It was too fast by about 0.5s when I tested it on 8 minutes
        simulation_time += (t - prev_t);
        update_simulation();
    }
    glutPostRedisplay();
}

void set_bars_color_mode(bars_color_mode_t mode)
{
    bars_color_mode = mode;
}