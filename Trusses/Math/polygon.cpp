//
//  polygon.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "polygon.h"

Polygon::Polygon() {}

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
    if (no_sides() < 3)
        throw std::length_error("number of sides is less than 3");
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

Vector2d Polygon::vertex_nomal(size_t vertex) const
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

double Polygon::area() const
{
    double ans = 0.0;
    
    int sides = (int)no_sides();
    for (int i = 0; i < sides; i++)
    {
        int next = (i == sides-1) ? 0 : i+1;
        
        ans -= (points[next].x - points[i].x) * (points[next].y + points[i].y) * 0.5;
        ans += (points[next].y - points[i].y) * (points[next].x + points[i].x) * 0.5;
    }
    ans /= 2.0;
    return ans;
}

bool Polygon::clockwise_winding() const
{
    return area() < 0.0;
}

bool Polygon::is_convex(std::vector<int>& ids, int vertex, bool clockw_winding) const
{
    int sides = (int)ids.size();
    if (vertex >= sides)
        throw std::invalid_argument("This vertex does not exist");
    
    int prev = (vertex == 0) ? sides-1 : vertex-1;
    int next = (vertex == sides-1) ? 0 : vertex+1;
    
    Vector2d v1 = points[ids[vertex]] - points[ids[prev]];
    Vector2d v2 = points[ids[next]] - points[ids[vertex]];
    if (clockw_winding)
        return v1.cross(v2) < 0;
    else
        return v1.cross(v2) > 0;
}

void Polygon::find_ear(std::vector<int>& ids, bool clockw_winding)
{
    bool ear_not_found = true;
    int sides = (int)ids.size();
    for (int i = 0; i < sides && ear_not_found; i++)
    {
        if (is_convex(ids, i, clockw_winding))
        {
            int prev = (i == 0) ? sides-1 : i-1;
            int next = (i == sides-1) ? 0 : i+1;
            
            Polygon triangle;
            triangle.add_point(points[ids[prev]]);
            triangle.add_point(points[ids[i]]);
            triangle.add_point(points[ids[next]]);
            
            // Check if there is any concave point in this triangle
            bool concave_inside = false;
            for (int j = 0; j < sides && !concave_inside; j++)
            {
                if (j == prev || j == i || j == next)
                    continue;
                if (!is_convex(ids, j, clockw_winding) && triangle.point_inside(points[ids[j]]))
                    concave_inside = true;
            }
            
            // Ear found
            if (!concave_inside)
            {
                ear_not_found = false;
                
                // Save the ear
                triangulation.push_back(ids[prev]);
                triangulation.push_back(ids[i]);
                triangulation.push_back(ids[next]);
                
                // Remove this vertex
                ids.erase(ids.begin()+i, ids.begin()+i+1);
            }
        }
    }
    
    // Signal that tomething is wrong
    if (ear_not_found)
        std::cout << "Warning: ear not found (at " << ids.size() << " sides)" << std::endl;
}

void Polygon::triangulate()
{
    // Start with the whole polygon (ids 0 to size()-1)
    std::vector<int> poly;
    poly.reserve(no_sides());
    for (int i = 0; i < no_sides(); i++)
        poly.push_back(i);
    
    // Triangulate the polygon. Limit the loop to no_sides()
    // iterations just in case.
    for (int i = 0; poly.size() > 2 && i < no_sides(); i++)
        find_ear(poly, clockwise_winding());
}
