//
//  wall_tool.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "wall_tool.h"
#include "polygon.h"
#include "mouse.h"
#include "interface.h"
#include "temporary_label.h"
#include "bars_tool.h"
#include "particle.h"
#include "wall.h"
#include "graphics.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#endif

void WallTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP)
        return;
    
    Vector2d mouse_pos = mouse.snap();
    
    if (points.size() != 2)
        points.push_back(Vector2d(mouse_pos.x, mouse_pos.y));
    if (points.size() == 2)
    {
        Wall::create(points[0], points[1]);
        Tool::set(current_tool, new BarsTool);
    }
}

void WallTool::passive()
{
    return;
}

void WallTool::drag()
{
    return;
}

void WallTool::display()
{
    Vector2d tool_pos = mouse.pos_world;
    
    // Snap the position vector
    if (mouse.particle_in_range())
        tool_pos = particles[mouse.closest_particle].position_;
    else if (mouse.grid_in_range())
        tool_pos = mouse.closest_grid;
    
    // Draw the current mouse position
    glLineWidth(1);
    glColor3f(WHITE);
    draw_cross(tool_pos, 10);
    
    // Draw the wall points
    for (int i = 0; i < points.size(); i++)
        draw_cross(points[i], 10);
}

void WallTool::key_down(unsigned char key)
{
    return;
}

WallTool::WallTool()
{
    TempLabel::create("Create a wall by defining two corner points", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
}

WallTool::~WallTool() {}