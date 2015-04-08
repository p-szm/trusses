//
//  drag_tool.h
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__drag_tool__
#define __Trusses__drag_tool__

#include "tool.h"
#include "math.h"
#include <vector>

class Renderer;

// For dragging particles in the simulation mode
class DragTool: public Tool
{
    friend class Renderer;
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display(const Renderer& rend);
    void key_down(unsigned char key);
    DragTool();
    ~DragTool();
private:
    std::vector<int> dragged_particles;
    double dragging_force; // Newtons/px
    
    // Reference position used to move fixed particles
    Vector2d mouse_previous;
};

#endif /* defined(__Trusses__drag_tool__) */
