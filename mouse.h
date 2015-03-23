//
//  mouse.h
//  Trusses
//
//  Created by Patrick on 23/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__mouse__
#define __Trusses__mouse__

#include <stdio.h>
#include <limits>
#include "math.h"

// Stores the position of the mouse in different coordinates
class Mouse
{
public:
    // In metres
    Vector2d pos_world = Vector2d(std::numeric_limits<float>::max(),
                                  std::numeric_limits<float>::max());
    
    // In pixels, centred at the centre
    Vector2d pos_screen = Vector2d(std::numeric_limits<float>::max(),
                                   std::numeric_limits<float>::max());
    
    // In UI coords (-1, 1)
    Vector2d pos_ui = Vector2d(std::numeric_limits<float>::max(),
                               std::numeric_limits<float>::max());
    
    // Updates pos_world, pos_gl and pos_screen variables from
    // the position of mouse in pixels (starting at top left corner).
    void update(int x, int y); // In pixels
    
    // The id of the closes particle
    int closest_particle = -1;
    
    // The position of the closest grid point
    Vector2d closest_grid = pos_world;
    
    // Minimum distance between the mouse and the point (in pixels)
    // for the point to be considered as "clicked".
    int min_click_dist = 10;
    
    // Checks the distance between the mouse and the point against min_click_dist
    bool in_range(Vector2d point);
    
    // Snaps to both particles (first) and grid,
    // or returns the original mouse position if the mouse
    // wasn't snapped.
    Vector2d snap();
    
    // True if the closest grid-point is in range
    bool grid_in_range();
    
    // True if the closest particle is in range
    bool particle_in_range();
};

extern Mouse mouse;

#endif /* defined(__Trusses__mouse__) */
