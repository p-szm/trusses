//
//  wall.h
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__wall__
#define __Trusses__wall__

#include <iostream>
#include "slot_map.h"

#include "math.h"

struct Wall
{
    Wall(Vector2d p1, Vector2d p2) {p1_ = p1; p2_ = p2;}
    Vector2d p1_;
    Vector2d p2_;
    int id_;
    
    double x_min() const;
    double x_max() const;
    double y_min() const;
    double y_max() const;
    
    static int create(Vector2d p1, Vector2d p2);
    static int destroy(int wall_id);
};

void reset_walls();

extern SlotMap<Wall> walls;

void print_walls();

#endif /* defined(__Trusses__wall__) */