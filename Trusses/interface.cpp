//
//  interface.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "interface.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#endif
#include <cstdlib>

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
Mouse mouse;
World world;

double grid_dist_px = 30.0; // In pixels
const double scroll_speed = 0.3; // gl/iteration
std::vector<int> selected_particles_ids;
int active_particle_id = -1;
bool drawing_wall = false;
std::vector<Vector2d> wall_points;
bool command_mode = false;
bool full_screen = false;
double dragging_force = 1000.0; // Newtons/px
bool simulation_paused = true;
std::vector<bool> arrows(4); // left, right, up, down

// * * * * * * * * * * //
bool in_range_m(double d1, double d2);

// * * * * * * * * * * //
Vector2d px_to_m(const Vector2d& v) {return Vector2d(px_to_m(v.x), px_to_m(v.y));}
double px_to_m(double d) {return d / world.scale;}
Vector2d px_to_ui(const Vector2d& v) {return Vector2d(px_to_ui_x(v.x), px_to_ui_y(v.y));}
double px_to_ui_x(double d) {return d / (window_width/2.0);}
double px_to_ui_y(double d) {return d / (window_height/2.0);}

// * * * * * * * * * * //
void key_down_function(unsigned char key, int x, int y)
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

void special_key_down(int key, int x, int y)
{
    // Remember which arrows were pressed
    if (key == GLUT_KEY_UP)
        arrows[2] = true;
    if (key == GLUT_KEY_DOWN)
        arrows[3] = true;
    if (key == GLUT_KEY_LEFT)
        arrows[0] = true;
    if (key == GLUT_KEY_RIGHT)
        arrows[1] = true;
    
    if (command_mode)
    {
        // If down arrow is pressed
        if (!arrows[0] && !arrows[1] && !arrows[2] && arrows[3] && current_cmd > 0)
            current_cmd--;
        // If up arrow is pressed
        else if (!arrows[0] && !arrows[1] && arrows[2] && !arrows[3] && current_cmd < commands.size() - 1)
            current_cmd++;
    }
    else
    {
        if (arrows[0])
            world.centre.x -= scroll_speed;
        if (arrows[1])
            world.centre.x += scroll_speed;
        if (arrows[2])
            world.centre.y += scroll_speed;
        if (arrows[3])
            world.centre.y -= scroll_speed;
    }
}

void special_key_up(int key, int x, int y)
{
    // Remove the arrow from the list of pressed arrows
    if (key == GLUT_KEY_UP)
        arrows[2] = false;
    if (key == GLUT_KEY_DOWN)
        arrows[3] = false;
    if (key == GLUT_KEY_LEFT)
        arrows[0] = false;
    if (key == GLUT_KEY_RIGHT)
        arrows[1] = false;
}

void editor_mouse(int button, int state, int x, int y)
{
    // Do nothing
    if (state == GLUT_UP)
        return;
    
    Vector2d snapped_point(mouse.pos_world);
    
    // See if any particle was clicked
    int clicked_particle_id = -1;
    int closest_part_id = mouse.closest_particle();
    if (closest_part_id != -1 && mouse.in_range(particles[closest_part_id].position_)) // A particle was clicked
        clicked_particle_id = closest_part_id;
    
    // Try to snap to the grid
    Vector2d closest_grid = mouse.closest_grid();
    if (clicked_particle_id == -1 && mouse.in_range(closest_grid))
        snapped_point = closest_grid;
    
    // Check if any button was pressed
    int pressed_button_id = -1;
    for (int i = 0; i < buttons.size() && pressed_button_id == -1; i++)
    {
        if (buttons[i].highlighted_)
        {
            buttons[i].execute_action();
            return;
        }
    }
    
    if (drawing_wall)
    {
        if (wall_points.size() != 2)
            wall_points.push_back(snapped_point);
        if (wall_points.size() == 2)
        {
            Wall::create(wall_points[0], wall_points[1]);
            wall_points.clear();
            drawing_wall = false;
            buttons[12].active_ = false; // TODO: what if this number changes?
        }
    }
    
    // Shift-click
    else if (state == GLUT_DOWN && glutGetModifiers() == GLUT_ACTIVE_SHIFT && clicked_particle_id != -1)
        Particle::destroy(clicked_particle_id);
    
    // If a particle was clicked
    else if (clicked_particle_id != -1)
        selected_particles_ids.push_back(clicked_particle_id);
    else
    {
        // Create a new particle and select it
        int new_p_id;
        if (button == GLUT_RIGHT_BUTTON)
            new_p_id = Particle::create(snapped_point.x, snapped_point.y, true); // Fixed
        else
            new_p_id = Particle::create(snapped_point.x, snapped_point.y, false); // Free
        
        selected_particles_ids.push_back(new_p_id);
    }
    
    // If precisely two particles are selected, create a bar between them and clear the vector of selected particles
    if (selected_particles_ids.size() == 2)
    {
        Bar::create(selected_particles_ids[0], selected_particles_ids[1], 0.0, ROOM_TEMPERATURE);
        selected_particles_ids.clear();
    }
    
    glutPostRedisplay();
}

void simulation_mouse(int button, int state, int x, int y)
{
    // Release a particle
    if (state == GLUT_UP && particles.exists(active_particle_id))
    {
        // Zero its acceleration
        particles[active_particle_id].external_acceleration_ = Vector2d(0.0, 0.0);
        active_particle_id = -1;
        return;
    }
    else if (state == GLUT_UP)
        return;
    
    int clicked_particle_id = -1;
    int closest_part_id = mouse.closest_particle();
    if (closest_part_id != -1 && mouse.in_range(particles[closest_part_id].position_)) // A particle was clicked
        clicked_particle_id = closest_part_id;
    
    // Check if any button was pressed
    int pressed_button_id = -1;
    for (int i = 0; i < buttons.size() && pressed_button_id == -1; i++)
    {
        if (buttons[i].highlighted_)
        {
            buttons[i].execute_action();
            return;
        }
    }
    
    // A particle was clicked and can be dragged around by mouse
    if (pressed_button_id == -1 && state == GLUT_DOWN && particles.exists(clicked_particle_id))
        active_particle_id = clicked_particle_id;
    
    glutPostRedisplay();
}

void editor_mouse_passive(int x, int y)
{
    mouse.update_from_px(x, y);
    highlight_buttons(mouse.pos_ui.x, mouse.pos_ui.y);
}

// When the mouse is just hovered
void simulation_mouse_passive(int x, int y)
{
    mouse.update_from_px(x, y);
    highlight_buttons(mouse.pos_ui.x, mouse.pos_ui.y);
}

void editor_mouse_drag(int x, int y)
{
    mouse.update_from_px(x, y);
}

void simulation_mouse_drag(int x, int y)
{
    mouse.update_from_px(x, y);
    
    if (particles.exists(active_particle_id))
    {
        // Particle is fixed
        if (particles[active_particle_id].fixed_)
            particles[active_particle_id].position_ = mouse.pos_world;
        // Particle is not fixed
        else
            particles[active_particle_id].external_acceleration_ =
                dragging_force * (mouse.pos_world - particles[active_particle_id].position_) / world.scale;
    }
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
    glutMouseFunc(editor_mouse);
    glutDisplayFunc(editor_display);
    glutIdleFunc(editor_idle);
    glutMotionFunc(editor_mouse_drag);
    glutPassiveMotionFunc(editor_mouse_passive);
    
    simulation_paused = true;
    
    temp_labels.clear();
    TempLabel::create("Editor mode - you can draw the structure. Press \"p\" when you are done", 0, 1.0, 0, -TOP_MARGIN, MODE_LABEL_TIME);
    
    buttons.clear();
    create_buttons_editor();
    
    mouse.min_click_dist = 10;
}

void resume_simulation()
{
    glutMouseFunc(simulation_mouse);
    glutDisplayFunc(simulation_display);
    glutIdleFunc(simulation_idle);
    glutMotionFunc(simulation_mouse_drag);
    glutPassiveMotionFunc(simulation_mouse_passive);
    
    microsecond_time(t);
    simulation_paused = false;
    
    temp_labels.clear();
    TempLabel::create("Simulation mode - you can drag the joints", 0, 1.0, 0, -TOP_MARGIN, MODE_LABEL_TIME);
    
    buttons.clear();
    create_buttons_simulation();
    
    mouse.min_click_dist = 20;
}

bool simulation_is_paused()
{
    return simulation_paused;
}

// True if two vectors (in metres) are closer than min_click_dist (in px)
bool in_range_m(double d1, double d2)
{
    if (abs_d(d1 - d2) * world.scale < mouse.min_click_dist)
        return true;
    return false;
}

// * * * * * * * * * * //
void register_callbacks()
{
    glutMouseFunc(editor_mouse);
    glutKeyboardFunc(key_down_function);
    //glutKeyboardUpFunc(key_up_function);
    glutIdleFunc(editor_idle);
    glutPassiveMotionFunc(editor_mouse_passive);
    glutMotionFunc(editor_mouse_drag);
    glutSpecialFunc(special_key_down);
    glutSpecialUpFunc(special_key_up);
}

void Mouse::update_from_px(int x, int y)
{
    pos_screen = Vector2d(x - window_width / 2.0, -y  + window_height / 2.0);
    pos_world = Vector2d(px_to_m(pos_screen.x) + world.centre.x, px_to_m(pos_screen.y) + world.centre.y);
    pos_ui = Vector2d(x * 2.0 / window_width - 1.0, 1.0 - y * 2.0 / window_height);
}

int Mouse::closest_particle()
{
    int closest_particle = -1;
    double least_dist2 = std::numeric_limits<float>::max();
    
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
    {
        double dist2_m = (pos_world - particles_it->position_).abs2();
        if (dist2_m < least_dist2)
        {
            closest_particle = particles_it->id_;
            least_dist2 = dist2_m;
        }
    }
    
    return closest_particle;
}

Vector2d Mouse::closest_grid()
{
    double grid_dist_m = grid_dist_px / world.scale;
    double closest_x = round(pos_world.x / grid_dist_m) * grid_dist_m;
    double closest_y = round(pos_world.y / grid_dist_m) * grid_dist_m;

    return Vector2d(closest_x, closest_y);
}

bool Mouse::in_range(Vector2d point)
{
    if ((point - pos_world).abs2() * world.scale * world.scale < min_click_dist * min_click_dist)
        return true;
    return false;
}
