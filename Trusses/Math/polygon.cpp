//
//  polygon.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "polygon.h"

void Polygon::add_point(const Vector2d &p)
{
    points.push_back(p);
}

size_t Polygon::no_sides() const
{
    if (points.size() < 3)
        return 0;
    return points.size();
}

bool Polygon::point_inside(const Vector2d& p) const
{
    bool inside = false;
    for (int i = 0; i < no_sides(); i++)
    {
        int j = (i + 1) % no_sides();
        Vector2d pi = points[i];
        Vector2d pj = points[j];
        // If the point lies between the y scope of the edge
        if ((p.y < pi.y) != (p.y < pj.y))
        {
            // Components of the vector representing the edge
            double x = pj.x - pi.x;
            double y = pj.y - pi.y;
            
            // Make sure it points up
            if (y < 0)
            {
                x = -x;
                y = -y;
            }
            
            // If the point lies to the left (use cross product)
            double cross = x * (p.y - pi.y) - y * (p.x - pi.x);
            if (cross > 0)
                inside = !inside;
            // TODO: what if cross is 0 ?
        }
    }
    return inside;
}

Vector2d Polygon::bounding_box_min() const
{
    Vector2d box_min = Vector2d(std::numeric_limits<double>::max(),
                                std::numeric_limits<double>::max());
    
    for (int i = 0; i < points.size(); i++)
    {
        if (points[i].x < box_min.x)
            box_min.x = points[i].x;
        if (points[i].y < box_min.y)
            box_min.y = points[i].y;
    }
    return box_min;
}

Vector2d Polygon::bounding_box_max() const
{
    Vector2d box_max = Vector2d(-std::numeric_limits<double>::max(),
                                -std::numeric_limits<double>::max());
    
    for (int i = 0; i < points.size(); i++)
    {
        if (points[i].x > box_max.x)
            box_max.x = points[i].x;
        if (points[i].y > box_max.y)
            box_max.y = points[i].y;
    }
    return box_max;
}

Vector2d Polygon::vertex_nomal(size_t vertex)
{
    if (no_sides() < 3)
        throw std::length_error("number of sides is less than 3");
    if (vertex >= no_sides())
        throw std::out_of_range("vertex does not exist");
    
    Vector2d edge1 = points[vertex] - points[vertex+1];
    Vector2d edge2;
    if (vertex > 0)
        edge2 = points[vertex] - points[vertex-1];
    else
        edge2 = points[0] - points.back();
    return Vector2d::bisect(edge1, edge2);
}