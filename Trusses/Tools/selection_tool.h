//
//  selection_tool.h
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__selection_tool__
#define __Trusses__selection_tool__

#include <stdio.h>
#include "tool.h"
#include <map>
#include "polygon.h"

// For selecting particles and deleting them
class SelectionTool: public Tool
{
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display();
    void key_down(unsigned char key);
    SelectionTool();
    ~SelectionTool();
private:
    Polygon poly;
    std::map<int, bool> selection_map;
};

#endif /* defined(__Trusses__selection_tool__) */
