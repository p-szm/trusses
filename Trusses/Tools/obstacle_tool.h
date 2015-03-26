//
//  obstacle_tool.h
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__obstacle_tool__
#define __Trusses__obstacle_tool__

#include <stdio.h>
#include "tool.h"
#include "polygon.h"

// For drawing obstacles
class ObstacleTool: public Tool
{
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display();
    void key_down(unsigned char key);
    ObstacleTool();
    ~ObstacleTool();
private:
    Polygon poly;
};

#endif /* defined(__Trusses__obstacle_tool__) */
