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
#include "graphics.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#endif

void DragTool::mouse_click(int button, int state)
{
    // Release a particle
    if (state == GLUT_UP && particles.exists(dragged_particle))
    {
        // Zero its acceleration
        particles[dragged_particle]->external_acceleration_ = Vector2d(0.0, 0.0);
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
    
    Particle* p = particles[dragged_particle];
    
    // Particle is fixed
    if (p->fixed_)
        p->position_ = mouse.pos_world;
    // Particle is not fixed
    else
        p->external_acceleration_ = dragging_force * (mouse.pos_world - p->position_) / world.scale;
}

void DragTool::display()
{
    bool highlighted = false;
    
    // Highlight a particle if it's close to the mouse
    if (mouse.particle_in_range())
    {
        Vector2d closest_pos = particles[mouse.closest_particle]->position_;
        glColor3f(GOLD);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2f(closest_pos.x, closest_pos.y);
        glEnd();
        highlighted = true;
    }
    
    // Draw the active particle
    if (!highlighted && particles.exists(dragged_particle))
    {
        Particle* active_p = particles[dragged_particle];
        Vector2d particle_pos_gl = active_p->position_ ;
        
        glColor3f(GOLD);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex2f(particle_pos_gl.x, particle_pos_gl.y);
        glEnd();
        
        glLineWidth(1.0);
        glBegin(GL_LINES);
        glVertex2f(particle_pos_gl.x, particle_pos_gl.y);
        glVertex2f(mouse.pos_world.x, mouse.pos_world.y);
        glEnd();
    }
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