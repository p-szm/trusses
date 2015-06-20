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
#include <vector>
#include "vector2d.h"

// Stores the position of the mouse in different coordinates
class Mouse
{
public:
    Mouse();
    
    // In metres
    Vector2d pos_world;
    
    // In pixels, centred at the centre
    Vector2d pos_screen;
    
    // In UI coords (-1, 1)
    Vector2d pos_ui;
    
    // Updates pos_world, pos_gl and pos_screen variables from
    // the position of mouse in pixels (starting at top left corner).
    void update(int x, int y); // In pixels
    
    // The id of the closes particle
    int closest_particle;
    
    // The position of the closest grid point
    Vector2d closest_grid;
    
    // Minimum distance between the mouse and the point (in pixels)
    // for the point to be considered as "clicked".
    int min_click_dist;
    
    // Checks the distance between the mouse and the point against min_click_dist
    bool in_range(Vector2d point) const;
    
    // Snaps to both particles (first) and grid,
    // or returns the original mouse position if the mouse
    // wasn't snapped.
    Vector2d snap();
    
    // True if the closest grid-point is in range.
    // If the grid is turned off it always returns false.
    bool grid_in_range() const;
    
    // True if the closest particle is in range
    bool particle_in_range() const;
    
    // Finds all the particles within the given range
    void particles_within(double dist, std::vector<int>& part) const;
    
    // Returns id of the closest bar within range, or -1 if none was found.
    int find_closest_bar(int px_range) const;
};

extern Mouse mouse;

#endif /* defined(__Trusses__mouse__) */
