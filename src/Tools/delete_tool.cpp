//
//  delete_tool.cpp
//  Trusses
//
//  Created by Patrick on 16/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "delete_tool.h"
#include "mouse.h"
#include "renderer.h"
#include "particle.h"
#include "bar.h"
#include "temporary_label.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void DeleteTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP)
        return;
    
    if (particles.exists(particle))
    {
        Particle::destroy(particle);
        particle = -1;
    }
    
    if (bars.exists(bar))
    {
        Bar::destroy(bar);
        bar = -1;
    }
}

void DeleteTool::passive()
{
    particle = -1;
    bar = -1;
    if (mouse.particle_in_range())
        particle = mouse.closest_particle;
    else
        bar = mouse.find_closest_bar(bar_range_px);
}

void DeleteTool::drag()
{
    return;
}

void DeleteTool::display(const Renderer& rend)
{
    rend.render(*this);
}

void DeleteTool::key_down(unsigned char key)
{
    return;
}

DeleteTool::DeleteTool()
{
    name = DELETE;
    particle = -1;
    bar = -1;
    bar_range_px = 20;
    TempLabel::create("Click on a particle/bar to delete it.", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
}

DeleteTool::~DeleteTool() {}