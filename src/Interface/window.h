//
//  window.w
//  Trusses
//
//  Created by Patrick on 08/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__window__
#define __Trusses__window__

#include "vector2d.h"

struct Arrows;

class Window
{
public:
    Window(double w, double h, double s);
    
    double get_scale() const;
    
    // Gradually zooms in/out until the target scale is reached.
    // The speed is set by the zoom_speed variable.
    void set_scale(double s);
    
    // Changes the scale by the multiplier d
    void scale_by(double d);
    
    // In px/metre
    int width;
    int height;
    bool full_screen;
    
    // The position in world coordinates of the centre of the screen
    Vector2d centre;
    
    // Responsible for moving thw view around
    void update(const Arrows& arr, const double dt);
    
    // In px per second
    int scroll_speed;
    
    void reset();
    
    // Return the coordinates of the visible world edges
    double left() const;
    double right() const;
    double bottom() const;
    double top() const;
    
private:
    double scale;
    double original_scale;
    double target_scale;
    
    // Between 0 (infinitely slow) and 1 (instantaneous)
    // This is one over number of frames that it takes to zoom
    double zoom_speed;
    
    double scale_min, scale_max;
};

extern Window window;

#endif /* defined(__Trusses__window__) */
