//
//  obstacle.h
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__obstacle__
#define __Trusses__obstacle__

#include <stdio.h>
#include <map> // temporary
#include "polygon.h"
#include "pointer_slot_map.h"

class Obstacle: public Polygon
{
public:
    Obstacle(const Polygon& poly);
    int id_;
    void draw();
    // Handle the collisions with the particles
    void collide();
private:
    Vector2d box_min;
    Vector2d box_max;
    void update_bounding_box();
};

extern PSlotMap<Obstacle*> obstacles;

#endif /* defined(__Trusses__obstacle__) */
