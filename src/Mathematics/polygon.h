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
#include "vector2d.h"

class Polygon
{
public:
    std::vector<Vector2d> points;
    
    Polygon();
    
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
    Vector2d vertex_nomal(size_t vertex) const;
    
    // Positive for anti-clockwise winding, negative
    // for clockwise winding. Uses Green's theorem.
    double area() const;
    
    // True if the polygon winding is clockwise, false
    // otherwise.
    bool clockwise_winding() const;
    
    // Checks for self-intersection. Complexity O(n2).
    bool self_intersects() const;
    
    // Triangulates the polygon and puts the result in
    // int the triangulation vector.
    void triangulate();
    
    // Ids of the triangulated polygon
    std::vector<int> triangulation;
private:
    // Returns the vertex which defines an ear
    void find_ear(std::vector<int>& ids, bool clockw_winding);
    
    // Works on the sub-polygon of the polygon
    bool is_convex(std::vector<int>& ids, int vertex, bool clockw_winding) const;
};

#endif /* defined(__Trusses__polygon__) */
