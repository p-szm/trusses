//
//  world.cpp
//  Trusses
//
//  Created by Patrick on 08/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "world.h"
#include "interface.h"

World world(50);

World::World(double s): scale(s), centre(Vector2d(0.0, 0.0)), scroll_speed(250) {}

void World::update_centre(const Arrows& arr, const double dt)
{
    if (arr.left)
        centre.x -= px_to_m(scroll_speed) * dt;
    if (arr.right)
        centre.x += px_to_m(scroll_speed) * dt;
    if (arr.up)
        centre.y += px_to_m(scroll_speed) * dt;
    if (arr.down)
        centre.y -= px_to_m(scroll_speed) * dt;
}