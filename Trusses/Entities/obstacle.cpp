//
//  obstacle.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "obstacle.h"
#include "particle.h"
#include "renderer.h"
#include <limits>

SlotMap<Obstacle> obstacles;

Obstacle::Obstacle(const Polygon& poly)
{
    // Copy the points
    points = poly.points;
    update_bounding_box();
}

void Obstacle::draw(const Renderer& rend)
{
    rend.render(*this);
}

void Obstacle::update_bounding_box()
{
    box_min = bounding_box_min();
    box_max = bounding_box_max();
}

void Obstacle::collide()
{
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles.at(i);
        
        // Continue if the particle is fixed
        if (p.fixed_)
            continue;
        
        // Continue if the particle is away from the obstacle
        Vector2d p_pos = p.position_;
        if (p_pos.x < box_min.x || p_pos.x > box_max.x ||
            p_pos.y < box_min.y || p_pos.y > box_max.y)
            continue;
        
        // Continue if the particle isn't inside the polygon
        if (!point_inside(p_pos))
            continue;

        // The particle is inside the polygon. Find the intersection point.
        // This is done by looping through all the edges of the polygon and computing
        // the intersection of this edge with the segment representing the change in
        // the particle's position (delta_p). The first intersection found is used.
        Segment delta_p = Segment(p.prev_position_verlet_, p.position_);
        bool intersected = false;
        for (int i = 0; i < points.size() && !intersected; i++)
        {
            Segment edge = (i == 0) ? Segment(points.back(), points[0]) : Segment(points[i-1], points[i]);
            
            double t, u;
            Vector2d intersection;
            intersected = delta_p.intersect(edge, intersection, t, u);
            
            // Intersection found
            if (intersected)
            {
                Vector2d edge_vect = edge.p1 - edge.p2;
                Vector2d edge_normal = Vector2d(-edge_vect.y, edge_vect.x).norm();
                
                // Reflect the particle over the edge
                Vector2d new_pos = p.position_.reflect(edge_normal, intersection);
                Vector2d new_prev_pos = p.prev_position_verlet_.reflect(edge_normal, intersection);
                
                // If the particle is still inside the polygon after the reflection,
                // it means that it probably bounced off the corner. In this case
                // just reverse its velocity (i.e. the angle of reflection is equal
                // to the angle of incidence).
                // TODO: This angle should be defined by the angle bisector of the two
                // near edges
                if (point_inside(new_pos))
                {
                    new_pos = 2 * intersection - p.position_;
                    new_prev_pos = 2 * intersection - p.prev_position_verlet_;
                }
                p.position_ = new_pos;
                p.prev_position_verlet_ = new_prev_pos;
            }
        }
    }
}