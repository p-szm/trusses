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
    // Release a particle
    if (state == GLUT_UP && particles.exists(dragged_particle))
    {
        // Zero its acceleration
        particles[dragged_particle].external_acceleration_ = Vector2d(0.0, 0.0);
        dragged_particle = -1;
        return;
    }
    else if (state == GLUT_UP)
        return;
    
    // A particle was clicked and can be dragged around by mouse
    if (mouse.particle_in_range())
        dragged_particle = mouse.closest_particle;
}

void DragTool::passive()
{
    return;
}

void DragTool::drag()
{
    if (!particles.exists(dragged_particle))
        return;
    
    Particle& p = particles[dragged_particle];
    
    // Particle is fixed
    if (p.fixed_)
        p.position_ = mouse.pos_world;
    // Particle is not fixed
    else
        p.external_acceleration_ = dragging_force * (mouse.pos_world - p.position_) / world.scale;
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
    TempLabel::create("Drag the joints around", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
}

DragTool::~DragTool() {}