//
//  trace_tool.h
//  Trusses
//
//  Created by Patrick on 14/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__trace_tool__
#define __Trusses__trace_tool__

#include "tool.h"

class TraceTool: public Tool
{
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display(const Renderer& rend);
    void key_down(unsigned char key);
    TraceTool();
    ~TraceTool();
};

#endif /* defined(__Trusses__trace_tool__) */
