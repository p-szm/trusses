//
//  interface.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//


#include <GLUT/glut.h>
#include <OpenGL/gl.h>

#include "graphics.h"
#include "interface.h"
#include "physics.h"
#include "save.h"
#include "interpreter.h"


bool snap = true;
bool snapped = false;
Vector2d snapped_point(0.0, 0.0);

bool drawing_wall = false;
std::vector<Vector2d> wall_points;

int min_click_dist = 10; // pixels
int selected_particle_id = -1;
int highlighted_particle_id = -1;

double scale = 40.0; // pixels/metre

Vector2d world_centre(0.0, 0.0); // In pixels

///////

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

///////

void key_pressed(unsigned char key, int x, int y)
{
    if (command_mode)
    {
        if (key == 27)
        {
            command_mode = false;
            command = "";
        }
        else if (key == 13)
        {
            command_mode = false;
            interpret_command(command);
            command = "";
        }
        else if (key == 127 || key == 8)
        {
            if (command.size() > 0)
                command = command.substr(0, command.size()-1);
        }
        else
        {
            command += key;
        }
    }
    else
    {
        if (key == 'g')
        {
            gravity = !gravity;
            std::cout << "Gravity: " << gravity << std::endl;
        }
        else if (key == 'o')
        {
            create_cloth(20, 0.1, Vector2d(0.0, 0.0), true);
        }
        else if (key == 'w')
        {
            Wall::destroy(2);
        }
        
        else if (key == 27)
        {
            std::exit(0);
        }
        
        else if (key == 13)
        {
            command_mode = true;
        }
    }
    glutPostRedisplay();
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
    bool highlighted_particle = false;
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end() && !highlighted_particle; particles_it++)
    {
        Vector2d p_pos = particles_it->position_;
        if (abs_d(x_metres - p_pos.x) < px_to_m(min_click_dist) && abs_d(y_metres - p_pos.y) < px_to_m(min_click_dist))
        {
            highlighted_particle_id = particles_it->id_;
            highlighted_particle = true;
        }
    }
    
    if (!highlighted_particle)
        highlighted_particle_id = -1;
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
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
    {
        Vector2d p_pos = particles_it->position_;
        if (abs_d(x_metres - p_pos.x) < px_to_m(min_click_dist) && abs_d(y_metres - p_pos.y) < px_to_m(min_click_dist) && button == GLUT_LEFT_BUTTON)
        {
            hit_particle_id = particles_it->id_;
            break;
        }
    }
    
    // Unselect particles
    if (state == GLUT_UP)
    {
        if (selected_particle_id != -1)
        {
            particles[selected_particle_id].external_acceleration_ = Vector2d(0.0, 0.0);
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
            Bar::create(selected_particle_id, hit_particle_id);
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
            int new_p_id;
            if (button == GLUT_RIGHT_BUTTON)
                new_p_id = Particle::create(x_metres, y_metres, true);
            else
                new_p_id = Particle::create(x_metres, y_metres, false);
            
            // Create a new bar
            Bar::create(selected_particle_id, new_p_id);
            selected_particle_id = -1;
        }
        
        // A click on an empty space
        else if (drawing_wall)
        {
            if (wall_points.size() != 2)
                wall_points.push_back(Vector2d(x_metres, y_metres));
            if (wall_points.size() == 2)
            {
                Wall::create(wall_points[0], wall_points[1]);
                wall_points.clear();
                drawing_wall = false;
                buttons[12].active_ = false; // TODO: what if this number changes?
            }
        }
        
        // A click on an empty space
        else
        {
            if (button == GLUT_RIGHT_BUTTON)
            {
                int new_p_id = Particle::create(x_metres, y_metres, true);
                if (particles[new_p_id].oscil_dir == CW || particles[new_p_id].oscil_dir == ACW)
                {
                    particles[new_p_id].oscillation_.origin.x = particles[new_p_id].position_.x - particles[new_p_id].oscillation_.amplitude;
                }
            }
            else
                Particle::create(x_metres, y_metres, false);
        }
        
        glutPostRedisplay();
    }
}

void mouse_drag(int x, int y)
{
    double x_px = x - window_width / 2.0; // wrt centre of the screen, not the world
    double y_px = -y  + window_height / 2.0;
    double x_metres = px_to_m(x_px - world_centre.x);
    double y_metres = px_to_m(y_px - world_centre.y);
    
    if (selected_particle_id != -1)
    {
        Particle* p = &particles[selected_particle_id];
        
        //p->position_ = Vector2d(x_metres, y_metres);
        //p->prev_position_ = p->position_;
        //p->prev_position_verlet_ = p->position_;
        Vector2d p_gl_pos = metres_to_gl_coords(p->position_);
        Vector2d mouse_gl_pos = metres_to_gl_coords(Vector2d(x_metres, y_metres));
        p->external_acceleration_ = 100 * Vector2d(mouse_gl_pos.x - p_gl_pos.x, mouse_gl_pos.y - p_gl_pos.y);
    }
}