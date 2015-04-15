//
//  split_tool.h
//  Trusses
//
//  Created by Patrick on 15/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__split_tool__
#define __Trusses__split_tool__

#include "tool.h"

class SplitTool: public Tool
{
    friend class Renderer;
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display(const Renderer& rend);
    void key_down(unsigned char key);
    SplitTool();
    ~SplitTool();
    
    // Number of parts the bar will be splitted to
    int parts;
    
private:
    int highlighted_bar;
    int selected_bar;
    
    int min_dist_px;
    int div_lines_px;
};

#endif /* defined(__Trusses__split_tool__) */
