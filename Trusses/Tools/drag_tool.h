//
//  drag_tool.h
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__drag_tool__
#define __Trusses__drag_tool__

#include <stdio.h>
#include "tool.h"

// For dragging particles in the simulation mode
class DragTool: public Tool
{
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display();
    void key_down(unsigned char key);
    DragTool();
    ~DragTool();
private:
    int dragged_particle = -1;
    double dragging_force = 1000.0; // Newtons/px
};

#endif /* defined(__Trusses__drag_tool__) */
