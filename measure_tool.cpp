//
//  measure_tool.cpp
//  Trusses
//
//  Created by Patrick on 16/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "measure_tool.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "renderer.h"
#include "particle.h"
#include "mouse.h"
#include "temporary_label.h"
#include "interface.h"

void MeasureTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP)
        return;
    
    // Measure something new
    if (selected_points.size() == 2)
    {
        selected_points.clear();
        distance = -1;
    }
    
    int clicked_p = -1;
    if (mouse.particle_in_range())
        clicked_p = mouse.closest_particle;
    
    // If a particle was clicked
    if (clicked_p != -1)
        selected_points.push_back(particles[clicked_p].position_);
    else
    {
        Vector2d mouse_pos = mouse.grid_in_range() ? mouse.closest_grid : mouse.pos_world;
        selected_points.push_back(mouse_pos);
    }
    
    // If we have two points calculate the distance
    if (selected_points.size() == 2)
        distance = (selected_points[0]-selected_points[1]).abs();
}

void MeasureTool::passive()
{
    return;
}

void MeasureTool::drag()
{
    return;
}

void MeasureTool::display(const Renderer& rend)
{
    rend.render(*this);
}

void MeasureTool::key_down(unsigned char key)
{
    return;
}

MeasureTool::MeasureTool()
{
    distance = -1;
    line_width_px = 20;
    TempLabel::create("Click a particle to trace it. Click again to stop tracing it.", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
}

MeasureTool::~MeasureTool() {}