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
#include "graphics.h"
#include "bars_tool.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
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
    SlotMap<Particle>::iterator p_it;
    for (p_it = particles.begin(); p_it != particles.end(); p_it++)
    {
        // TODO: Only particles that are lose to the selection
        // should be checked.
        if (poly.point_inside(p_it->position_))
            selection_map[p_it->id_] = true;
        else
            selection_map[p_it->id_] = false;
    }
}

void SelectionTool::display()
{
    // Draw the polygon
    glColor3f(GOLD);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < poly.no_sides(); i++)
        glVertex2f(poly.points[i].x, poly.points[i].y);
    glEnd();
    
    // Highlight the selected points
    glPointSize(10);
    glBegin(GL_POINTS);
    for (int i = 0; i < selection_map.size(); i++)
    {
        if (selection_map[i])
        {
            Vector2d pos = particles[i].position_;
            glVertex2d(pos.x, pos.y);
        }
    }
    glEnd();
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
    std::cout << "BarsTool " << this << " created" << std::endl;
}

SelectionTool::~SelectionTool()
{
    std::cout << "BarsTool " << this << " destroyed" << std::endl;
}