//
//  wall.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "wall.h"

std::vector<Wall> walls;
int walls_number = 0;

Wall Wall::create(Vector2d p1, Vector2d p2)
{
    Wall wl = Wall(p1, p2);
    
    wl.id = walls_number;
    walls_number ++;
    
    return wl;
}

double Wall::x_min() const
{
    return min(p1_.x, p2_.x);
}

double Wall::x_max() const
{
    return max(p1_.x, p2_.x);
}

double Wall::y_min() const
{
    return min(p1_.y, p2_.y);
}

double Wall::y_max() const
{
    return max(p1_.y, p2_.y);
}