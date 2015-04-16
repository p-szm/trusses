//
//  measure_tool.h
//  Trusses
//
//  Created by Patrick on 16/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__measure_tool__
#define __Trusses__measure_tool__

#include "tool.h"
#include "math.h"
#include <vector>

class MeasureTool: public Tool
{
    friend class Renderer;
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display(const Renderer& rend);
    void key_down(unsigned char key);
    MeasureTool();
    ~MeasureTool();
private:
    std::vector<Vector2d> selected_points;
    double distance;
    int line_width_px;
};

#endif /* defined(__Trusses__measure_tool__) */
