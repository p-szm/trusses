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
#include "renderer.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
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

void WallTool::display(const Renderer& rend)
{
    rend.render(*this);
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