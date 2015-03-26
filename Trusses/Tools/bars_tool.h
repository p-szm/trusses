//
//  bars_tool.h
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__bars_tool__
#define __Trusses__bars_tool__

#include <stdio.h>
#include "tool.h"
#include <vector>

// For drawing particles and bars
class BarsTool: public Tool
{
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display();
    void key_down(unsigned char key);
    BarsTool();
    ~BarsTool();
private:
    std::vector<int> selected_particles_ids;
};

#endif /* defined(__Trusses__bars_tool__) */
