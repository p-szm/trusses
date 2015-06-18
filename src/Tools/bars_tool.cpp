//
//  bars_tool.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "bars_tool.h"
#include "mouse.h"
#include "bar.h"
#include "particle.h"
#include "renderer.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
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
    
    // If a particle was clicked
    if (clicked_particle_id != -1)
        selected_particles_ids.push_back(clicked_particle_id);
    else
    {
        // Create a new particle and select it
        int new_p_id;
        if (button == GLUT_RIGHT_BUTTON)
            new_p_id = Particle::create(tool_pos.x, tool_pos.y, true); // Fixed
        else
            new_p_id = Particle::create(tool_pos.x, tool_pos.y, false); // Free
        
        selected_particles_ids.push_back(new_p_id);
    }
    
    // If precisely two particles are selected, create a bar between them and clear the vector of selected particles
    if (selected_particles_ids.size() == 2)
    {
        Bar::create(selected_particles_ids[0], selected_particles_ids[1]);
        selected_particles_ids.clear();
    }
}

void BarsTool::passive()
{
    tool_pos = mouse.pos_world;
    snapped = false;
    
    if (mouse.particle_in_range())
    {
        tool_pos = particles[mouse.closest_particle].position_;
        snapped = true;
    }
    else if (mouse.grid_in_range())
    {
        tool_pos = mouse.closest_grid;
        snapped = true;
    }
}

void BarsTool::drag()
{
    return;
}

void BarsTool::display(const Renderer& rend)
{
    rend.render(*this);
}

void BarsTool::key_down(unsigned char key)
{
    return;
}

BarsTool::BarsTool()
{
    name = BARS;
    tool_pos = Vector2d(0, 0);
    snapped = false;
}

BarsTool::~BarsTool() {}