//
//  wall.h
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__wall__
#define __Trusses__wall__

#include <stdio.h>
#include "pointer_slot_map.h"
#include "math.h"

class Renderer;

struct Wall
{
    Vector2d p1_;
    Vector2d p2_;
    int id_;
    
    double x_min() const;
    double x_max() const;
    double y_min() const;
    double y_max() const;
    
    // Handles the collisions with the particles
    void collide();
    
    void draw(const Renderer& rend) const;
    
    static int create(Vector2d p1, Vector2d p2);
    static int destroy(int wall_id);
private:
    Wall(Vector2d p1, Vector2d p2) {p1_ = p1; p2_ = p2;}
};

void print_walls();

extern PSlotMap<Wall*> walls;

#endif /* defined(__Trusses__wall__) */