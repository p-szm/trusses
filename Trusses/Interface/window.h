//
//  window.w
//  Trusses
//
//  Created by Patrick on 08/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__window__
#define __Trusses__window__

#include "math.h"

class Arrows;

class Window
{
public:
    Window(double s, double w, double h);
    
    // In px/metre
    double scale;
    int width;
    int height;
    
    // The position in world coordinates of the centre of the screen
    Vector2d centre;
    void update_centre(const Arrows& arr, const double dt);
    
    int scroll_speed;
};

extern Window window;

#endif /* defined(__Trusses__window__) */
