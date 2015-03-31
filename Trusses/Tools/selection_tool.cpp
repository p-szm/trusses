//
//  selection_tool.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "selection_tool.h"
#include "mouse.h"
#include "interface.h"
#include "temporary_label.h"
#include "particle.h"
#include "bars_tool.h"
#include "renderer.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define SELECTION_RES 10

void SelectionTool::mouse_click(int button, int state)
{
    return;
}

void SelectionTool::passive()
{
    return;
}

void SelectionTool::drag()
{
    // Add new points to the selection polygon
    Vector2d pos = mouse.pos_world;
    double res = px_to_m(SELECTION_RES);
    if (poly.no_sides() == 0 || abs_d(pos.x - poly.points.back().x) > res || abs_d(pos.y - poly.points.back().y) > res)
        poly.add_point(Vector2d(pos.x, pos.y));
    
    // For each particle decide if it lies inside or outside the polygon.
    // Use a map to record results (so it's easy to avoid adding the same particle multiple times)
    for (int i = 0; i < particles.size(); i++)
    {
        if (poly.no_sides() < 3)
            return;
        
        Particle& p = particles.at(i);
        // TODO: Only particles that are lose to the selection
        // should be checked.
        if (poly.point_inside(p.position_))
            selection_map[p.id_] = true;
        else
            selection_map[p.id_] = false;
    }
}

void SelectionTool::display(const Renderer& rend)
{
    rend.render(*this);
}

void SelectionTool::key_down(unsigned char key)
{
    // Backspace
    if (key == 127)
    {
        // Destroy each selected particle
        for (int i = 0; i < selection_map.size(); i++)
        {
            if (selection_map[i])
                Particle::destroy(i);
        }
        // Update the selection map
        selection_map.clear();
        
        // Switch the tool
        Tool::set(current_tool, new BarsTool);
    }
}

SelectionTool::SelectionTool()
{
    TempLabel::create("Create a selection by dragging", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
}

SelectionTool::~SelectionTool() {}