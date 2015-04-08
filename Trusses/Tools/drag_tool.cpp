//
//  drag_tool.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "drag_tool.h"
#include "mouse.h"
#include "interface.h"
#include "world.h"
#include "temporary_label.h"
#include "particle.h"
#include "renderer.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void DragTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP)
    {
        // Release dragged particles (zero their acceleration)
        for (int i = 0; i < dragged_particles.size(); i++)
        {
            int p_id = dragged_particles[i];
            if (particles.exists(p_id))
                particles[p_id].external_acceleration_ = Vector2d(0.0, 0.0);
        }
        dragged_particles.clear();
    }
    else
    {
        // A particle was clicked and can be dragged around by mouse
        mouse.particles_within(px_to_m(mouse.min_click_dist), dragged_particles);
        mouse_previous = mouse.pos_world;
    }
}

void DragTool::passive()
{
    return;
}

void DragTool::drag()
{
    size_t no_part = dragged_particles.size();
    for (int i = 0; i < no_part; i++)
    {
        // TODO: See if it sometimes happen that the particle doesn't exist.
        int p_id = dragged_particles[i];
        if (!particles.exists(p_id))
            return;
        
        Particle& p = particles[p_id];
        if (p.fixed_)
        {
            Vector2d delta_pos = mouse.pos_world - mouse_previous;
            p.position_ += delta_pos;
        }
        else
        {
            p.external_acceleration_ = dragging_force * (mouse.pos_world - p.position_) / (world.scale * no_part);
        }
    }
    mouse_previous = mouse.pos_world;
}

void DragTool::display(const Renderer& rend)
{
    rend.render(*this);
}

void DragTool::key_down(unsigned char key)
{
    return;
}

DragTool::DragTool()
{
    dragging_force = 1000.0;
    TempLabel::create("Drag the joints around", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
}

DragTool::~DragTool() {}