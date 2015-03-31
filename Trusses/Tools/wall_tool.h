//
//  wall_tool.h
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__wall_tool__
#define __Trusses__wall_tool__

#include <stdio.h>
#include "tool.h"
#include <vector>
#include "math.h"

class Renderer;

// For drawing walls
class WallTool: public Tool
{
    friend class Renderer;
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display(const Renderer& rend);
    void key_down(unsigned char key);
    WallTool();
    ~WallTool();
private:
    std::vector<Vector2d> points;
};

#endif /* defined(__Trusses__wall_tool__) */
