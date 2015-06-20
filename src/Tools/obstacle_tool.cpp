//
//  obstacle_tool.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "obstacle_tool.h"
#include "mouse.h"
#include "interface.h"
#include "temporary_label.h"
#include "bars_tool.h"
#include "obstacle.h"
#include "renderer.h"
#include "various_math.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define SELECTION_RES 15

void ObstacleTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP && poly.no_sides() >= 5)
    {
        // Add a copy to the obstacles container
        // TODO: Copying this might be slow
        Obstacle::create(poly);
        Tool::set(current_tool, new BarsTool);
    }
}

void ObstacleTool::passive()
{
    return;
}

void ObstacleTool::drag()
{
    // Add new points to the polygon
    Vector2d pos = mouse.pos_world;
    double res = px_to_m(SELECTION_RES);
    if (poly.no_sides() == 0 || abs_d(pos.x - poly.points.back().x) > res || abs_d(pos.y - poly.points.back().y) > res)
        poly.add_point(Vector2d(pos.x, pos.y));
}

void ObstacleTool::display(const Renderer& rend)
{
    rend.render(*this);
}

void ObstacleTool::key_down(unsigned char key)
{
    return;
}

ObstacleTool::ObstacleTool()
{
    name = OBSTACLE;
    TempLabel::create("Draw an obstacle", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
}

ObstacleTool::~ObstacleTool() {}