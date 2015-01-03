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

bool snap = true;
bool snapped = false;
Vector2d snapped_point(0.0, 0.0);

bool drawing_wall = false;
std::vector<Vector2d> wall_points;

int min_click_dist = 10; // pixels
int selected_particle_id = -1;
int highlighted_particle_id = -1;

double scale = 150.0; // pixels/metre

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
    else if (key == 's')
    {
        std::string path = "/Users/patrick/Desktop/save-";
        path += date_str() + '-' + time_str();
        save(path);
    }
    else if (key == 'p')
    {
        load("/Users/patrick/Desktop/save-2015-1-3-21-14-12");
    }
    else if (key == 'o')
    {
        create_cloth(20, 0.1, Vector2d(0.0, 0.0), false);
    }
    std::cout << 1 << std::endl;
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
    for (int i = 0; i < particles_number && !highlighted_particle; i++)
    {
        Vector2d p_pos = particles[i].position_;
        if (abs_d(x_metres - p_pos.x) < px_to_m(min_click_dist) && abs_d(y_metres - p_pos.y) < px_to_m(min_click_dist))
        {
            highlighted_particle_id = particles[i].id_;
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
    for (int i = 0; i < particles_number; i++)
    {
        Vector2d p_pos = particles[i].position_;
        if (abs_d(x_metres - p_pos.x) < px_to_m(min_click_dist) && abs_d(y_metres - p_pos.y) < px_to_m(min_click_dist) && button == GLUT_LEFT_BUTTON)
        {
            hit_particle_id = particles[i].id_;
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

void mouse_drag(int x, int y)
{
    double x_coord = (x - window_width / 2.0)/scale;
    double y_coord = (-y  + window_height / 2.0)/scale;
    
    if (selected_particle_id != -1)
    {
        Vector2d pos = particles[selected_particle_id].position_;
        particles[selected_particle_id].external_acceleration_ = 100 * Vector2d(x_coord - pos.x, y_coord - pos.y);
    }
}