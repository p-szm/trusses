//
//  mouse.cpp
//  Trusses
//
//  Created by Patrick on 23/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "mouse.h"
#include "window.h"
#include "interface.h"
#include "grid.h"
#include "particle.h"
#include "bar.h"
#include <limits>
#include "settings.h"
#include "segment.h"

Mouse mouse;

Mouse::Mouse()
{
    pos_world = Vector2d(std::numeric_limits<float>::max(),
                                  std::numeric_limits<float>::max());
    pos_screen = Vector2d(std::numeric_limits<float>::max(),
                                   std::numeric_limits<float>::max());
    pos_ui = Vector2d(std::numeric_limits<float>::max(),
                               std::numeric_limits<float>::max());
    closest_particle = -1;
    min_click_dist = 10;
    closest_grid = pos_world;
}

void Mouse::update(int x, int y)
{
    pos_screen = Vector2d(x - window.width / 2.0, -y  + window.height / 2.0);
    pos_world = Vector2d(px_to_m(pos_screen.x) + window.centre.x, px_to_m(pos_screen.y) + window.centre.y);
    pos_ui = Vector2d(x * 2.0 / window.width - 1.0, 1.0 - y * 2.0 / window.height);
    
    // Update the closest particle
    double least_dist2 = std::numeric_limits<float>::max();
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles.at(i);
        double dist2_m = (pos_world - p.position_).abs2();
        if (dist2_m < least_dist2)
        {
            closest_particle = p.id_;
            least_dist2 = dist2_m;
        }
    }
    
    // Update the closest grid
    double grid_dist_m = grid.spacing / window.get_scale();
    double closest_x = round(pos_world.x / grid_dist_m) * grid_dist_m;
    double closest_y = round(pos_world.y / grid_dist_m) * grid_dist_m;
    closest_grid = Vector2d(closest_x, closest_y);
}

bool Mouse::in_range(Vector2d point) const
{
    if ((point - pos_world).abs2() * window.get_scale() * window.get_scale() < min_click_dist * min_click_dist)
        return true;
    return false;
}

Vector2d Mouse::snap()
{
    if (particle_in_range())
        return particles[closest_particle].position_;
    else if (grid_in_range())
        return closest_grid;
    return mouse.pos_world;
}

bool Mouse::grid_in_range() const
{
    if (!settings.get(GRID))
        return false;
    
    if (in_range(closest_grid))
        return true;
    
    return false;
}

bool Mouse::particle_in_range() const
{
    if (!particles.exists(closest_particle))
        return false;
    if (in_range(particles[closest_particle].position_))
        return true;
    return false;
}

void Mouse::particles_within(double dist, std::vector<int>& part) const
{
    double dist2 = dist * dist;
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles.at(i);
        double dist2_m = (pos_world - p.position_).abs2();
        if (dist2_m < dist2)
            part.push_back(p.id_);
    }
}

int Mouse::find_closest_bar(int px_range) const
{
    int bar = -1;
    double m_range = px_to_m(px_range);
    double smallest_dist2 = std::numeric_limits<double>::max();
    for (int i = 0; i < bars.size(); i++)
    {
        Bar& b = bars.at(i);
        
        // Check if the point is "within" the bar
        Vector2d p1 = particles[b.p2_id].position_;
        Vector2d p2 = particles[b.p1_id].position_;
        
        if ((p2-p1)*(mouse.pos_world-p1) > 0 && (p1-p2)*(mouse.pos_world-p2) > 0)
        {
            // Check the distance
            Segment seg(p1, p2);
            
            double dist2 = seg.dist2(mouse.pos_world);
            if (dist2 < m_range * m_range && dist2 < smallest_dist2)
            {
                bar = b.id_;
                smallest_dist2 = dist2;
            }
        }
    }
    return bar;
}