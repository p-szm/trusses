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
bool snap = true;

bool drawing_wall = false;
std::vector<Vector2d> wall_points;

int min_click_dist = 15; // pixels
double scale = 150.0; // pixels/metre

int selected_particle_id = -1;

bool snapped = false;
Vector2d snapped_point(0.0, 0.0);

// In pixels
Vector2d world_centre(0.0, 0.0);

extern std::vector<Particle> particles;
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
        x_px = m_to_px(x_metres);
        y_px = m_to_px(y_metres);
    }
    else
    {
        x_px = x - window_width / 2.0; // wrt centre of the screen, not the world
        y_px = -y  + window_height / 2.0;
        x_metres = px_to_m(x_px - world_centre.x);
        y_metres = px_to_m(y_px - world_centre.y);
    }
    
    // See if any particle was hit
    int hit_particle_id = -1;
    for (int i = 0; i < particles_number; i++)
    {
        Vector2d p_pos = particles[i].position;
        if (abs_d(x_metres - p_pos.x) < px_to_m(min_click_dist) && abs_d(y_metres - p_pos.y) < px_to_m(min_click_dist) && button == GLUT_LEFT_BUTTON)
        {
            hit_particle_id = particles[i].id;
            break;
        }
    }
    
    // Unselect particles
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
        bool button_pressed = false;
        
        // Check if any button was pressed
        for (int i = 0; i < buttons_number && button_pressed == false; i++)
        {
            if (buttons[i].highlighted_)
            {
                buttons[i].execute_action();
                std::cout << "Button " << buttons[i].id_ <<" was clicked" << std::endl;
                button_pressed = true;
            }
        }

        if (button_pressed)
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
        else if (drawing_wall)
        {
            if (wall_points.size() != 2)
                wall_points.push_back(Vector2d(x_metres, y_metres));
            if (wall_points.size() == 2)
            {
                walls.push_back(Wall::create(wall_points[0], wall_points[1]));
                wall_points.clear();
                drawing_wall = false;
                buttons[12].active_ = false; // TODO: what if this number changes?
            }
        }
        
        // A click on an empty space
        else
        {
            if (button == GLUT_RIGHT_BUTTON)
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
        std::string path = "/Users/patrick/Desktop/save-";
        path += date_str() + '-' + time_str();
        save(path);
    }
    else if (key == 'p')
    {
        load("/Users/patrick/Desktop/cantilever.txt");
    }
    else if (key == 'i')
    {
        ids = !ids;
    }
    else if (key == 'w')
    {
        walls.push_back(Wall::create(Vector2d(0.0, -1.0), Vector2d(1.0, 1.0)));
    }
    else if (key == 'o')
    {
        create_cloth(20, 0.1, Vector2d(0.0, 0.0), false);
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
    double x_px = x - window_width / 2.0; // wrt centre of the screen, not the world
    double y_px = -y  + window_height / 2.0;
    double x_metres = px_to_m(x_px - world_centre.x);
    double y_metres = px_to_m(y_px - world_centre.y);
    
    double closest_x = round(x_metres);
    double closest_y = round(y_metres);
    
    //std::cout << x_metres << ' ' << y_metres << std::endl;
    
    // Highlight the buttons
    bool button_hit = false;
    for (int i = 0; i < buttons_number && button_hit == false; i++)
    {
        if (buttons[i].is_hit(x_px, y_px))
        {
            buttons[i].highlighted_ = true;
            button_hit = true;
        }
        else
        {
            buttons[i].highlighted_ = false;
        }
    }
    
    // Snap to the grid
    if (snap)
    {
        if (abs_d(closest_x - x_metres) < px_to_m(min_click_dist) && abs_d(closest_y - y_metres) < px_to_m(min_click_dist))
        {
            snapped = true;
            snapped_point = Vector2d(closest_x, closest_y);
        }
        else
        {
            snapped = false;
        }
    }
    
    // Highlight particles
    for (int i = 0; i < particles_number; i++)
    {
        Vector2d p_pos = particles[i].position;
        if (abs_d(x_metres - p_pos.x) < px_to_m(min_click_dist) && abs_d(y_metres - p_pos.y) < px_to_m(min_click_dist))
        {
            particles[i].highlight = true;
        }
        else
            particles[i].highlight = false;
    }
}

void mouse_drag(int x, int y)
{
    double x_coord = (x - window_width / 2.0)/scale;
    double y_coord = (-y  + window_height / 2.0)/scale;
    
    if (selected_particle_id != -1)
    {
        Vector2d pos = particles[selected_particle_id].position;
        particles[selected_particle_id].external_acceleration = 10 * Vector2d(x_coord - pos.x, y_coord - pos.y);
    }
}

void special_key_down(int key, int x, int y)
{
    // Arrows move the world around
    if (key == GLUT_KEY_UP)
    {
        world_centre.y -= 10; // px
    }
    else if (key == GLUT_KEY_DOWN)
    {
        world_centre.y += 10;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        world_centre.x += 10;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        world_centre.x -= 10;
    }
}

//////////

void draw_particle(const Particle& p)
{
    Vector2d pos = p.position;
    Vector2d pos_gl = metres_to_gl_coords(pos);
    
    // If selected
    if (selected_particle_id == p.id)
    {
        glColor3f(1.0, 1.0, 0.0);
        glPointSize(10);
    }
    // If fixed
    else if (p.fixed)
    {
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(5);
    }
    else
    {
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(5);
    }
    
    // Increase the point size if highlighted and not selected
    if (p.highlight && selected_particle_id != p.id)
    {
        glPointSize(10);
    }
    
    glBegin(GL_POINTS);
    glVertex2f(pos_gl.x, pos_gl.y);
    glEnd();
    
    if (ids)
    {
        std::stringstream s;
        s << p.id;
        
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
    
    glLineWidth(2.0);
    
    Vector2d gl_start = metres_to_gl_coords(particles[b.p1_id].position);
    Vector2d gl_end = metres_to_gl_coords(particles[b.p2_id].position);
    
    Vector2d m_mid = 0.5 * (particles[b.p1_id].position + particles[b.p2_id].position);
    
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
    if (extension_rates)
    {
        s.str("");
        s << b.extension_rate();
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

Vector2d m_to_px(Vector2d v)
{
    return scale * v;
}

double m_to_px(double d)
{
    return d * scale;
}

Vector2d px_to_m(Vector2d v)
{
    return (1/scale) * v;
}

double px_to_m(double d)
{
    return d / scale;
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
    
    
    
}