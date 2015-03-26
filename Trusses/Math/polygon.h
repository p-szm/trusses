//
//  polygon.h
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__polygon__
#define __Trusses__polygon__

#include <stdio.h>
#include <vector>
#include "math.h"

class Polygon
{
public:
    std::vector<Vector2d> points;
    
    // Adds the point after the previous last point.
    void add_point(const Vector2d& p);
    
    // Returns 0 if there are less than 3 points
    // defining the polygon.
    size_t no_sides() const;
    
    // True if the point is inside the polygon.
    bool point_inside(const Vector2d& p) const;
    
    // Bottom left corner of the smallest rectangle
    // containing the whole polygon.
    Vector2d bounding_box_min() const;
    
    // Top right corner of the smallest rectangle
    // containing the whole polygon.
    Vector2d bounding_box_max() const;
    
    // Vertex normal pointing away from the polygon
    // for acute angles.
    Vector2d vertex_nomal(size_t vertex);
};

#endif /* defined(__Trusses__polygon__) */
