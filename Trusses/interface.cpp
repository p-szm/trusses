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

bool drawing_wall = false;
std::vector<Vector2d> wall_points;

int min_click_dist = 10; // pixels
int selected_particle_id = -1;
int highlighted_particle_id = -1;

double scale = 40.0; // pixels/metre

Vector2d world_centre(0.0, 0.0); // In pixels

Vector2d mouse_pos; // In world coords

bool command_mode = false;

bool full_screen = false;

struct Snap
{
    
};

class Interface
{
public:
    bool drawing_wall;
};



// Represents d pixels in metres
double px_to_m(double d)
{
    return d / scale;
}

void key_pressed(unsigned char key, int x, int y)
{
    if (command_mode)
    {
        if (key == 27)
        {
            command_mode = false;
            commands.back() = "";
        }
        else if (key == 13)
        {
            command_mode = false;
            std::string cmd_to_execute = commands[commands.size() - current_cmd - 1];
            if (cmd_to_execute != "")
            {
                interpret_command(cmd_to_execute);
                commands.back() = cmd_to_execute;
            }
            current_cmd = 0;
        }
        else if (key == 127 || key == 8)
        {
            if (commands.back().size() > 0 && current_cmd == 0)
                commands.back() = commands.back().substr(0, commands.back().size()-1);
        }
        else
        {
            if (current_cmd == 0)
                commands.back() += key;
        }
    }
    else
    {
        if (key == 'g')
            gravity = !gravity;
        else if (key == 'o')
            create_cloth(20, 0.1, Vector2d(0.0, 0.0), false);
        else if (key == 27)
            std::exit(0);
        else if (key == 13)
        {
            command_mode = true;
            if (commands.size() == 0 || commands.back() != "")
                commands.push_back("");
        }
        if (key == 'c')
        {
            if (bars_color_mode == STRAIN_C)
                bars_color_mode = TEMP_C;
            else if (bars_color_mode == TEMP_C)
                bars_color_mode = STRAIN_C;
        }
        if (key == 'f')
        {
            full_screen = !full_screen;
            if (full_screen)
                glutFullScreen();
            else
            {
                glutReshapeWindow(1100, 750);
                glutPositionWindow(100, 80);
            }
        }
    }
    
    // Redraw the window
    glutPostRedisplay();
}

void special_key_down(int key, int x, int y)
{
    // Arrows move the world around
    if (key == GLUT_KEY_UP)
    {
        if (command_mode)
        {
            if (current_cmd < commands.size() - 1)
                current_cmd++;
        }
        else
            world_centre.y -= 10; // px
    }
    else if (key == GLUT_KEY_DOWN)
    {
        if (command_mode)
        {
            if (current_cmd > 0)
                current_cmd--;
        }
        else
            world_centre.y += 10;
    }
    else if (key == GLUT_KEY_LEFT)
        world_centre.x += 10;
    else if (key == GLUT_KEY_RIGHT)
        world_centre.x -= 10;
}

// True if two vectors (in metres) are closer than min_click_dist (in px)
bool in_range_m(double d1, double d2)
{
    if (abs_d(d1 - d2) * scale < min_click_dist)
        return true;
    return false;
}

// When the mouse is just hovered
void mouse_passive(int x, int y)
{
    double x_px = x - window_width / 2.0; // Wrt the centre of the screen, not the world
    double y_px = -y  + window_height / 2.0;
    double x_metres = px_to_m(x_px - world_centre.x); // In the worlds coordinates
    double y_metres = px_to_m(y_px - world_centre.y);
    
    double closest_x = round(x_metres);
    double closest_y = round(y_metres);
    
    // Remember mouse position
    mouse_pos = Vector2d(x_metres, y_metres);
    
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
            buttons[i].highlighted_ = false;
    }
    
    // Snap to the grid
    if (snap)
    {
        if (in_range_m(closest_x, x_metres) && in_range_m(closest_y, y_metres))
        {
            snapped = true;
            mouse_pos = Vector2d(closest_x, closest_y);
        }
        else
            snapped = false;
    }
    
    // Highlight particles
    bool highlighted_particle = false;
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end() && !highlighted_particle; particles_it++)
    {
        if (in_range_m(x_metres, particles_it->position_.x) && in_range_m(y_metres, particles_it->position_.y))
        {
            highlighted_particle_id = particles_it->id_;
            highlighted_particle = true;
        }
    }
    
    // -1 if none of the particles were highlighted
    if (!highlighted_particle)
        highlighted_particle_id = -1;
}

void mouse_click (int button, int state, int x, int y)
{
    // GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON
    // GLUT_UP or GLUT_DOWN
    
    double x_metres, y_metres;
    
    // Move the mouse to the snapped point if snapping is enabled and
    if (snap && snapped)
    {
        x_metres = mouse_pos.x;
        y_metres = mouse_pos.y;
    }
    else
    {
        double x_px = x - window_width / 2.0; // Wrt to centre of the screen, not the world
        double y_px = -y  + window_height / 2.0;
        x_metres = px_to_m(x_px - world_centre.x); // In world coordinates
        y_metres = px_to_m(y_px - world_centre.y);
    }
    
    // See if any particle was clicked, and if it was grab the first one that's detected
    int clicked_particle_id = -1;
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end() && clicked_particle_id == -1; particles_it++)
    {
        Vector2d p_pos = particles_it->position_;
        if (abs_d(x_metres - p_pos.x) < px_to_m(min_click_dist) && abs_d(y_metres - p_pos.y) < px_to_m(min_click_dist) && button == GLUT_LEFT_BUTTON)
            clicked_particle_id = particles_it->id_;
    }
    
    // Unselect particles
    if (state == GLUT_UP)
    {
        if (selected_particle_id != -1)
            particles[selected_particle_id].external_acceleration_ = Vector2d(0.0, 0.0);
        
        if (clicked_particle_id == -1)
            selected_particle_id = -1;
    }
    
    else if (state == GLUT_DOWN)
    {
        // Check if any button was pressed
        int pressed_button_id = -1;
        for (int i = 0; i < buttons_number && pressed_button_id == -1; i++)
            if (buttons[i].highlighted_)
                pressed_button_id = buttons[i].id_;
        
        // If a button was pressed
        if (pressed_button_id != -1)
            buttons[pressed_button_id].execute_action();
        
        // If a particle was clicked and one was already selected
        else if (clicked_particle_id != -1 && selected_particle_id != -1)
        {
            Bar::create(selected_particle_id, clicked_particle_id);
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
        Vector2d p_gl_pos = metres_to_gl_coords(p->position_);
        Vector2d mouse_gl_pos = metres_to_gl_coords(Vector2d(x_metres, y_metres));
        p->external_acceleration_ = 100 * Vector2d(mouse_gl_pos.x - p_gl_pos.x, mouse_gl_pos.y - p_gl_pos.y);
    }
}