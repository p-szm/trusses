//
//  tool.h
//  Trusses
//
//  Created by Patrick on 22/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__tool__
#define __Trusses__tool__

#include <stdio.h>

class Renderer;

// In world coordinates
class Tool
{
public:
    virtual void mouse_click(int button, int state) = 0;
    virtual void passive() = 0;
    virtual void drag() = 0;
    virtual void display(const Renderer& rend) = 0;
    virtual void key_down(unsigned char key) = 0;
    
    static void set(Tool*& tool, Tool* new_tool);
    virtual ~Tool();
};

#endif /* defined(__Trusses__tool__) */
