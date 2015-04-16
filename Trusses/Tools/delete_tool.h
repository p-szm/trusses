//
//  delete_tool.h
//  Trusses
//
//  Created by Patrick on 16/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__delete_tool__
#define __Trusses__delete_tool__

#include "tool.h"
#include "math.h"

class DeleteTool: public Tool
{
    friend class Renderer;
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display(const Renderer& rend);
    void key_down(unsigned char key);
    DeleteTool();
    ~DeleteTool();
private:
    int particle;
    int bar;
    int bar_range_px;
};

#endif /* defined(__Trusses__delete_tool__) */
