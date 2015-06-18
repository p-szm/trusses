//
//  trace_tool.cpp
//  Trusses
//
//  Created by Patrick on 14/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "trace_tool.h"
#include "temporary_label.h"
#include "renderer.h"
#include "mouse.h"
#include "particle.h"
#include "bars_tool.h"
#include "interface.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void TraceTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP)
        return;
    
    int clicked_p = -1;
    if (mouse.particle_in_range())
        clicked_p = mouse.closest_particle;
    
    if (clicked_p != -1)
    {
        if (particles[clicked_p].traced())
            particles[clicked_p].untrace();
        else
            particles[clicked_p].trace();
    }
}

void TraceTool::passive()
{
    return;
}

void TraceTool::drag()
{
    return;
}

void TraceTool::display(const Renderer& rend)
{
    rend.render(*this);
}

void TraceTool::key_down(unsigned char key)
{
    if (key == 13)
        Tool::set(current_tool, new BarsTool);
}

TraceTool::TraceTool()
{
    name = TRACE;
    TempLabel::create("Click a particle to trace it. Click again to stop tracing it.", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
}

TraceTool::~TraceTool() {}