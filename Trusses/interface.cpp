//
//  interface.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "interface.h"

#include <GLUT/glut.h>
#include <OpenGL/gl.h>

#include "particle.h"
#include "bar.h"
#include "wall.h"
#include "button.h"
#include "graphics.h"
#include "physics.h"
#include "save.h"
#include "interpreter.h"
#include "temporary_label.h"

// * * * * * * * * * * //
double scale = 40.0; // pixels/metre
Vector2d world_centre(0.0, 0.0); // In pixels
bool snap = true;
bool snapped = false;
std::vector<int> selected_particles_ids;
int active_particle_id = -1;
bool drawing_wall = false;
std::vector<Vector2d> wall_points;
Vector2d mouse_pos; // In world coords
bool command_mode = false;
bool full_screen = false;
double dragging_force = 100.0;
int min_click_dist = 10; // pixels
bool simulation_paused = true;

// * * * * * * * * * * //
bool in_range_m(double d1, double d2);

// * * * * * * * * * * //
Vector2d m_to_gl(Vector2d v) {return Vector2d(2 * v.x * scale / window_width, 2 * v.y * scale / window_height);}

// * * * * * * * * * * //
void key_function(unsigned char key, int x, int y)
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
            create_cloth(20, 0.5, Vector2d(0.0, 0.0), false);
        else if (key == 27)
            std::exit(0);
        else if (key == 13)
        {
            command_mode = true;
            if (commands.size() == 0 || commands.back() != "")
                commands.push_back("");
        }
        else if (key == 'c')
        {
            if (bars_color_mode == STRAIN_C)
                set_bars_color_mode(TEMP_C);
            else if (bars_color_mode == TEMP_C)
                set_bars_color_mode(STRAIN_C);
        }
        else if (key == 'f')
        {
            full_screen = !full_screen;
            if (full_screen)
            {
                glutFullScreen();
                // TODO: Labels have to update their position
            }
            else
            {
                glutReshapeWindow(1100, 750);
                glutPositionWindow(100, 80);
            }
        }
        else if (key == 'b')
            fancy_bars = !fancy_bars;
        else if (key == 's')
            show_particles = !show_particles;
        else if (key == 'p')
        {
            if (simulation_is_paused())
                resume_simulation();
            else
                pause_simulation();
        }
    }
    
    // Redraw the window
    glutPostRedisplay();
}

void special_key_function(int key, int x, int y)
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

void mouse_function(int button, int state, int x, int y)
{
    bool drawing_mode = simulation_is_paused();
    
    // Release a particle
    if (state == GLUT_UP && !drawing_mode)
    {
        // Zero its acceleration
        particles[active_particle_id].external_acceleration_ = Vector2d(0.0, 0.0);
        active_particle_id = -1;
        return;
    }
    else if (state == GLUT_UP)
        return;
    
    // If snapping is enabled, mouse_pos is changed accordingly, so no need to think about snapping here
    double x_metres = mouse_pos.x;
    double y_metres = mouse_pos.y;
    
    // Check if any particle was clicked, and if it was grab the first one that's detected
    int clicked_particle_id = -1;
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end() && clicked_particle_id == -1; particles_it++)
    {
        Vector2d p_pos = particles_it->position_;
        if (abs_d(x_metres - p_pos.x) < px_to_m(min_click_dist) && abs_d(y_metres - p_pos.y) < px_to_m(min_click_dist) && button == GLUT_LEFT_BUTTON)
            clicked_particle_id = particles_it->id_;
    }
    
    // Check if any button was pressed
    int pressed_button_id = -1;
    for (int i = 0; i < buttons_number && pressed_button_id == -1; i++)
        if (buttons[i].highlighted_)
            pressed_button_id = buttons[i].id_;
    
    // Execute the pressed button's action (if any was pressed)
    if (pressed_button_id != -1)
    {
        buttons[pressed_button_id].execute_action();
        return;
    }
    
    // Drawing mode
    if (simulation_is_paused() && state == GLUT_DOWN)
    {
        if (drawing_wall)
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
        
        // If a particle was clicked
        else if (clicked_particle_id != -1)
            selected_particles_ids.push_back(clicked_particle_id);
        
        // If empty space was clicked
        else
        {
            // Create a new particle and select it
            int new_p_id;
            if (button == GLUT_RIGHT_BUTTON)
                new_p_id = Particle::create(x_metres, y_metres, true); // Fixed
            else
                new_p_id = Particle::create(x_metres, y_metres, false); // Free
            
            selected_particles_ids.push_back(new_p_id);
        }
        
        // If precisely two particles are selected, create a bar between them and clear the vector of selected particles
        if (selected_particles_ids.size() == 2)
        {
            Bar::create(selected_particles_ids[0], selected_particles_ids[1], 0.0, ROOM_TEMPERATURE);
            selected_particles_ids.clear();
        }
    }
    
    // // A particle was clicked and can be dragged around by mouse
    else if (state == GLUT_DOWN && particles.exists(clicked_particle_id))
        active_particle_id = clicked_particle_id;
    
    
    glutPostRedisplay();
}

// When the mouse is just hovered
void mouse_passive_function(int x, int y)
{
    double x_px = x - window_width / 2.0; // Wrt the centre of the screen, not the world
    double y_px = -y  + window_height / 2.0;
    double x_metres = px_to_m(x_px - world_centre.x); // In the worlds coordinates
    double y_metres = px_to_m(y_px - world_centre.y);
    
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
    
    snapped = false;
    
    // Snap to a particle if it's close
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
    {
        if (in_range_m(x_metres, particles_it->position_.x) && in_range_m(y_metres, particles_it->position_.y))
        {
            snapped = true;
            mouse_pos = particles_it->position_;
        }
    }
    
    // If it's not snapped yet, snap to the grid (only in drawing mode)
    if (!snapped && snap && simulation_is_paused())
    {
        double closest_x = round(x_metres);
        double closest_y = round(y_metres);
        
        if (in_range_m(closest_x, x_metres) && in_range_m(closest_y, y_metres))
        {
            snapped = true;
            mouse_pos = Vector2d(closest_x, closest_y);
        }
        else
            snapped = false;
    }
}

void mouse_drag_function(int x, int y)
{
    double x_px = x - window_width / 2.0; // Wrt the centre of the screen, not the world
    double y_px = -y  + window_height / 2.0;
    double x_metres = px_to_m(x_px - world_centre.x); // In the worlds coordinates
    double y_metres = px_to_m(y_px - world_centre.y);
    mouse_pos = Vector2d(x_metres, y_metres);
    
    // Unsnap everything
    snapped = false;
    
    bool drawing_mode = simulation_is_paused();
    if (drawing_mode)
    {
        
    }
    else // Simulation mode
    {
        if (particles.exists(active_particle_id))
        {
            if (particles[active_particle_id].fixed_)
                particles[active_particle_id].position_ = mouse_pos;
            else
                particles[active_particle_id].external_acceleration_ = dragging_force * m_to_gl(mouse_pos - particles[active_particle_id].position_);
        }
    }
    //double x_metres = px_to_m(x_px - world_centre.x);
    //double y_metres = px_to_m(y_px - world_centre.y);
    
    /*if (selected_particle_id != -1)
     {
     Particle* p = &particles[selected_particle_id];
     if (p->fixed_)
     {
     p->position_ = Vector2d(x_metres, y_metres);
     p->prev_position_ = p->position_;
     }
     else
     {
     Vector2d p_gl_pos = world_to_gl_coords(p->position_);
     Vector2d mouse_gl_pos = world_to_gl_coords(Vector2d(x_metres, y_metres));
     p->external_acceleration_ = 100 * Vector2d(mouse_gl_pos.x - p_gl_pos.x, mouse_gl_pos.y - p_gl_pos.y);
     }
     }*/
}

void start_drawing_wall()
{
    drawing_wall = true;
    selected_particles_ids.clear();
}

void stop_drawing_wall()
{
    drawing_wall = false;
    wall_points.clear();
}

void pause_simulation()
{
    simulation_paused = true;
    temp_labels.clear();
    TempLabel::create("Editor mode - you can draw the structure", Vector2d(-110, window_height/2 - 20), 5000000); // 5s
}

void resume_simulation()
{
    microsecond_time(t);
    simulation_paused = false;
    temp_labels.clear();
    TempLabel::create("Simulation mode - you can drag the joints", Vector2d(-110, window_height/2 - 20), 5000000); // 5s
}

bool simulation_is_paused()
{
    return simulation_paused;
}

// True if two vectors (in metres) are closer than min_click_dist (in px)
bool in_range_m(double d1, double d2)
{
    if (abs_d(d1 - d2) * scale < min_click_dist)
        return true;
    return false;
}