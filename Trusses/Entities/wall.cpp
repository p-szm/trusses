//
//  wall.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "wall.h"
#include "temporary_label.h"
#include "physics.h" // For ENERGY_ABSORPTION
#include "renderer.h"

SlotMap<Wall> walls;

int Wall::create(Vector2d p1, Vector2d p2)
{
    if (abs_d(p1.x - p2.x) < SMALL_NUM || abs_d(p1.y - p2.y) < SMALL_NUM)
    {
        issue_label("Cannot create a wall", WARNING_LABEL_TIME);
        return 1;
    }
    int new_id = walls.add(Wall(p1, p2));
    return new_id;
}

int Wall::destroy(int obj_id)
{
    int result = walls.remove(obj_id);
    return result;
}

double Wall::x_min() const
{
    return min(p1_.x, p2_.x);
}

double Wall::x_max() const
{
    return max(p1_.x, p2_.x);
}

double Wall::y_min() const
{
    return min(p1_.y, p2_.y);
}

double Wall::y_max() const
{
    return max(p1_.y, p2_.y);
}

void print_walls()
{
    for (int i = 0; i < walls.size(); i++)
    {
        std::cout << "Wall " << walls.at(i).id_ << std::endl;
    }
}

void Wall::collide() const
{
    double eng_regained_perp = 1 - ENERGY_ABSORPTION;
    double eng_regained_par = 1 - 0.1 * ENERGY_ABSORPTION; // Determines friction between a particle and a walls
    
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles.at(i);
        
        // TODO: Think about fixed particles: they shouldn't be checked all the time, just when they are dragged
        // For now:
        if (p.fixed_)
            continue;
        
        double wall_top = y_max();
        double wall_bottom = y_min();
        double wall_left = x_min();
        double wall_right = x_max();
        
        Vector2d p_pos = p.position_;
        Vector2d p_prev_pos = p.prev_position_verlet_;
        Vector2d p_vel = p.velocity_;
        
        // Particle penetrated the bottom wall
        if (p_prev_pos.y <= wall_bottom && p_pos.y >= wall_bottom && p_prev_pos.x >= wall_left && p_prev_pos.x <= wall_right)
        {
            p.position_.y = wall_bottom - SMALL_NUM;
            p.velocity_ = Vector2d(eng_regained_par * p_vel.x, -eng_regained_perp * p_vel.y);
            
            // So it doesn't break the Verlet integrator
            p.prev_position_verlet_ = p_pos - delta_t * p_vel;
        }
        
        // Particle penetrated the left wall
        else if (p_prev_pos.x <= wall_left && p_pos.x >= wall_left && p_prev_pos.y <= wall_top && p_prev_pos.y >= wall_bottom)
        {
            p.position_.x = wall_left - SMALL_NUM;
            p.velocity_ = Vector2d(-eng_regained_perp * p_vel.x, eng_regained_par * p_vel.y);
            p.prev_position_verlet_ = p_pos - delta_t * p_vel;
        }
        
        // Particle penetrated the right wall
        else if (p_prev_pos.x >= wall_right && p_pos.x <= wall_right && p_prev_pos.y <= wall_top && p_prev_pos.y >= wall_bottom)
        {
            p.position_.x = wall_right + SMALL_NUM;
            p.velocity_ = Vector2d(-eng_regained_perp * p_vel.x, eng_regained_par * p_vel.y);
            p.prev_position_verlet_ = p_pos - delta_t * p_vel;
        }
        
        // Particle penetrated the top wall
        else if (p_prev_pos.y >= wall_top && p_pos.y <= wall_top && p_prev_pos.x >= wall_left && p_prev_pos.x <= wall_right)
        {
            p.position_.y = wall_top + SMALL_NUM;
            p.velocity_ = Vector2d(eng_regained_par * p_vel.x, -eng_regained_perp * p_vel.y);
            p.prev_position_verlet_ = p_pos - delta_t * p_vel;
        }
    }
}

void Wall::draw(const Renderer& rend) const
{
    rend.render(*this);
}