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
    bool full_screen;
    
    // The position in world coordinates of the centre of the screen
    Vector2d centre;
    void update_centre(const Arrows& arr, const double dt);
    
    // In px per second
    int scroll_speed;
    
    void reset();
    
    // Return the coordinates of the visible world edges
    double left() const;
    double right() const;
    double bottom() const;
    double top() const;
    
private:
    double original_scale;
};

extern Window window;

#endif /* defined(__Trusses__window__) */
