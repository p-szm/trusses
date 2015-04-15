//
//  split_tool.cpp
//  Trusses
//
//  Created by Patrick on 15/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "split_tool.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "mouse.h"
#include "temporary_label.h"
#include "interface.h"
#include "interpreter.h"
#include "game.h"
#include "renderer.h"
#include "bar.h"
#include "particle.h"
#include "bars_tool.h"
#include "math.h"
#include <limits>
#include <sstream>
#include <string>

void SplitTool::mouse_click(int button, int state)
{
    if (state == GLUT_UP)
        return;
    
    selected_bar = highlighted_bar;
    highlighted_bar = -1;
    TempLabel::create("Type in a number. Press ENTER to split and BACKSPACE to undo typing.", 0, 1, 0, -TOP_MARGIN, INFO_LABEL_TIME);
}

void SplitTool::passive()
{
    // Return if the bar is already selected
    if (selected_bar != -1)
        return;
    
    double min_dist = px_to_m(min_dist_px);
    
    highlighted_bar = -1;
    double smallest_dist2 = std::numeric_limits<double>::max();
    for (int i = 0; i < bars.size(); i++)
    {
        Bar& b = bars.at(i);
        
        // Check if the point is "within" the bar
        Vector2d p1 = particles[b.p2_id].position_;
        Vector2d p2 = particles[b.p1_id].position_;
        
        if ((p2-p1)*(mouse.pos_world-p1) > 0 && (p1-p2)*(mouse.pos_world-p2) > 0)
        {
            // Check the distance
            Segment seg(p1, p2);
            
            double dist2 = seg.dist2(mouse.pos_world);
            if (dist2 < min_dist * min_dist && dist2 < smallest_dist2)
            {
                highlighted_bar = b.id_;
                smallest_dist2 = dist2;
            }
        }
    }
}

void SplitTool::drag()
{
    return;
}

void SplitTool::display(const Renderer& rend)
{
    rend.render(*this);
}

void SplitTool::key_down(unsigned char key)
{
    if (!bars.exists(selected_bar))
        return;
    
    if (key == 13)
    {
        if (parts != 0)
            Bar::split(selected_bar, parts);
        parts = 1;
        selected_bar = -1;
        Tool::set(current_tool, new BarsTool);
    }
    if (key == 127 || key == 8)
    {
        // Remove a digit
        parts /= 10;
    }
    else
    {
        std::string key_str = std::string(1, key);
        if (Interpreter::is_number(key_str))
        {
            // Add a digit to the number of parts
            int number = Interpreter::get_number<int>(key_str);
            int new_parts = 10*parts + number;
            if (new_parts <= max_parts)
                parts = new_parts;
            else
            {
                std::stringstream message;
                message << "Too many parts (max " << max_parts << ")";
                issue_label(message.str(), WARNING_LABEL_TIME);
            }
        }
    }
}

SplitTool::SplitTool(): max_parts(100)
{
    min_dist_px = 20;
    div_lines_px = 40;
    highlighted_bar = -1;
    selected_bar = -1;
    parts = 0;
    TempLabel::create("Select a bar to split", 0, 1, 0, -TOP_MARGIN, TOOL_LABEL_TIME);
}

SplitTool::~SplitTool() {}