//
//  world.h
//  Trusses
//
//  Created by Patrick on 08/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__world__
#define __Trusses__world__

#include "math.h"

class Arrows;

class World
{
public:
    World(double s);
    
    // In px/metre
    double scale;
    
    // The position in world coordinates of the centre of the screen
    Vector2d centre;
    
    void update_centre(const Arrows& arr, const double dt);
    
    int scroll_speed;
};

extern World world;

#endif /* defined(__Trusses__world__) */
