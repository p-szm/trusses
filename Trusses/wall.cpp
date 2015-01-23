//
//  wall.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "wall.h"

SlotMap<Wall> walls;

int Wall::create(Vector2d p1, Vector2d p2)
{
    Wall new_wall(p1, p2);
    int new_id = walls.add(new_wall);
    
    return new_id;
}

int Wall::destroy(int obj_id)
{
    int result = walls.remove(obj_id);
    return result;
}

void reset_walls()
{
    walls.clear();
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

void print_walls()
{
    SlotMap<Wall>::iterator walls_it;
    for (walls_it = walls.begin(); walls_it != walls.end(); walls_it++)
    {
        std::cout << "Wall " << walls_it->id_ << std::endl;
    }
}