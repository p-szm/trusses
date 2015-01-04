//
//  wall.h
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Oscillator__wall__
#define __Oscillator__wall__

#include <iostream>
#include <vector>

#include "math.h"
#include "physics.h"

struct Wall
{
    Wall(Vector2d p1, Vector2d p2) {p1_ = p1; p2_ = p2;}
    Vector2d p1_;
    Vector2d p2_;
    ID id_;
    
    double x_min() const;
    double x_max() const;
    double y_min() const;
    double y_max() const;
    
    static ID create(Vector2d p1, Vector2d p2);
    static void destroy(ID wall_id);
    static int get_current_version(int number);
};

void reset_walls();

extern std::vector<Wall> walls;
extern int walls_number;

#endif /* defined(__Oscillator__wall__) */
