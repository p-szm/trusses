//
//  bars_tool.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "bars_tool.h"
#include "mouse.h"
#include "pin_joint.h"
#include "bar.h"
#include "physics.h"
#include "graphics.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#endif

void BarsTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP)
        return;
    
    int clicked_particle_id = -1;
    if (mouse.particle_in_range())
        clicked_particle_id = mouse.closest_particle;
    
    // Shift-click destroys the clicked particle
    if (state == GLUT_DOWN && glutGetModifiers() == GLUT_ACTIVE_SHIFT && clicked_particle_id != -1)
        Particle::destroy(clicked_particle_id);
    
    // If a particle was clicked
    else if (clicked_particle_id != -1)
        selected_particles_ids.push_back(clicked_particle_id);
    else
    {
        Vector2d mouse_pos = mouse.grid_in_range() ? mouse.closest_grid : mouse.pos_world;
        
        // Create a new particle and select it
        int new_p_id;
        if (button == GLUT_RIGHT_BUTTON)
            new_p_id = PinJoint::create(mouse_pos.x, mouse_pos.y, true); // Fixed
        else
            new_p_id = PinJoint::create(mouse_pos.x, mouse_pos.y, false); // Free
        
        selected_particles_ids.push_back(new_p_id);
    }
    
    // If precisely two particles are selected, create a bar between them and clear the vector of selected particles
    if (selected_particles_ids.size() == 2)
    {
        Bar::create(selected_particles_ids[0], selected_particles_ids[1], 0.0, ROOM_TEMPERATURE);
        selected_particles_ids.clear();
    }
}

void BarsTool::passive()
{
    return;
}

void BarsTool::drag()
{
    return;
}

void BarsTool::display()
{
    Vector2d tool_pos = mouse.pos_world;
    
    // Snap the position vector
    bool snapped = true;
    if (mouse.particle_in_range())
        tool_pos = particles[mouse.closest_particle]->position_;
    else if (mouse.grid_in_range())
        tool_pos = mouse.closest_grid;
    else
        snapped = false;
    
    // Draw dashed lines between the selected particles and the mouse
    if (selected_particles_ids.size() != 0)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
        glLineWidth(1.0);
        glColor3f(GOLD);
        glBegin(GL_LINES);
        for (int i = 0; i < selected_particles_ids.size(); i++)
        {
            if (particles.exists(selected_particles_ids[i])) // Make sure that this particle exists
            {
                Vector2d selected_pos = particles[selected_particles_ids[i]]->position_;
                glVertex2f(selected_pos.x, selected_pos.y);
                glVertex2f(tool_pos.x, tool_pos.y);
            }
        }
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    }
    
    if (snapped)
    {
        glColor3f(GOLD);
        glPointSize(10);
        draw_point(tool_pos);
    }
}

void BarsTool::key_down(unsigned char key)
{
    return;
}

BarsTool::BarsTool()
{
    std::cout << "BarsTool " << this << " created" << std::endl;
}

BarsTool::~BarsTool()
{
    std::cout << "BarsTool " << this << " destroyed" << std::endl;
}