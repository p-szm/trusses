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

Mouse mouse;

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
    double grid_dist_m = grid.grid_dist_px / window.scale;
    double closest_x = round(pos_world.x / grid_dist_m) * grid_dist_m;
    double closest_y = round(pos_world.y / grid_dist_m) * grid_dist_m;
    closest_grid = Vector2d(closest_x, closest_y);
}

bool Mouse::in_range(Vector2d point) const
{
    if ((point - pos_world).abs2() * window.scale * window.scale < min_click_dist * min_click_dist)
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