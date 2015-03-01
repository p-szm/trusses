//
//  particle.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "particle.h"
#include "physics.h"
#include "bar.h"
#include "wall.h"

SlotMap<Particle> particles;

int Particle::create(double a, double b, bool fixed)
{
    Particle new_particle(a, b);
    
    new_particle.velocity_ = Vector2d(0.0, 0.0);
    new_particle.prev_position_ = new_particle.position_ - delta_t * new_particle.velocity_;
    new_particle.prev_position_verlet_ = new_particle.position_ - delta_t * new_particle.velocity_;
    new_particle.acceleration_ = Vector2d(0.0, 0.0);
    new_particle.external_acceleration_ = Vector2d(0.0, 0.0);
    new_particle.mass_ = 1.0;
    
    new_particle.fixed_ = (fixed) ? true : false;
    new_particle.dragged_ = false;
    
    // Trace
    new_particle.trace_on = false;
    
    int new_id = particles.add(new_particle);
    
    return new_id;
}

int Particle::destroy(int obj_id)
{
    if (!particles.exists(obj_id))
    {
        std::cout << "This particle doesn't exist" << std::endl;
        return 1;
    }
    
    // Remove all the bars that this particle was connected to
    Particle* this_p = &particles[obj_id];
    while (this_p->bars_connected.size() > 0)
        Bar::destroy(this_p->bars_connected.back());
    
    int result = particles.remove(obj_id);
    
    return result;
}

void Particle::impose_boundaries()
{
    double eng_regained_perp = 1 - ENERGY_ABSORPTION;
    double eng_regained_par = 1 - 0.1 * ENERGY_ABSORPTION; // Determines friction between a particle and a walls
    
    SlotMap<Wall>::iterator walls_it;
    for (walls_it = walls.begin(); walls_it != walls.end(); walls_it++)
    {
        // TODO: Think about fixed particles: they shouldn't be checked all the time, just when they are dragged
        // For now:
        if (fixed_)
            continue;
        
        double wall_top = walls_it->y_max();
        double wall_bottom = walls_it->y_min();
        double wall_left = walls_it->x_min();
        double wall_right = walls_it->x_max();
        
        // Particle penetrated the bottom wall
        if (prev_position_.y <= wall_bottom && position_.y >= wall_bottom && prev_position_.x >= wall_left && prev_position_.x <= wall_right)
        {
            position_.y = wall_bottom - SMALL_NUM;
            velocity_ = Vector2d(eng_regained_par * velocity_.x, -eng_regained_perp * velocity_.y);
            
            // So it doesn't break the Verlet integrator
            prev_position_verlet_ = position_ - delta_t * velocity_;
        }
        
        // Particle penetrated the left wall
        else if (prev_position_.x <= wall_left && position_.x >= wall_left && prev_position_.y <= wall_top && prev_position_.y >= wall_bottom)
        {
            position_.x = wall_left - SMALL_NUM;
            velocity_ = Vector2d(-eng_regained_perp * velocity_.x, eng_regained_par * velocity_.y);
            prev_position_verlet_ = position_ - delta_t * velocity_;
        }
        
        // Particle penetrated the right wall
        else if (prev_position_.x >= wall_right && position_.x <= wall_right && prev_position_.y <= wall_top && prev_position_.y >= wall_bottom)
        {
            position_.x = wall_right + SMALL_NUM;
            velocity_ = Vector2d(-eng_regained_perp * velocity_.x, eng_regained_par * velocity_.y);
            prev_position_verlet_ = position_ - delta_t * velocity_;
        }
        
        // Particle penetrated the top wall
        else if (prev_position_.y >= wall_top && position_.y <= wall_top && prev_position_.x >= wall_left && prev_position_.x <= wall_right)
        {
            position_.y = wall_top + SMALL_NUM;
            velocity_ = Vector2d(eng_regained_par * velocity_.x, -eng_regained_perp * velocity_.y);
            prev_position_verlet_ = position_ - delta_t * velocity_;
        }
    }
}

void Particle::update()
{
    if (!fixed_)
    {
        // External acceleration is the acceleration added by dragging with a mouse
        acceleration_ = external_acceleration_;
        
        // Remember the presious position
        prev_position_ = position_;
        
        // Trace the previous position
        if (trace_on)
            trace.add(position_);
        
        if (gravity)
            acceleration_ += Vector2d(0.0, -GRAVITY);
        
        // Verlet integration
        Vector2d next_position = 2 * position_ - prev_position_verlet_ + pow(delta_t, 2) * acceleration_;
        velocity_ = (0.5 / delta_t) * (next_position - prev_position_);
        prev_position_verlet_ = position_;
        position_ = next_position;
    }
}

void print_particles()
{
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
    {
        std::cout << "Particle " << particles_it->id_ << ": ";
        for (int j = 0; j < particles_it->bars_connected.size(); j++)
        {
            std::cout << particles_it->bars_connected[j] << " ";
        }
        std::cout << std::endl;
    }
}

void reset_particles()
{
    particles.clear();
}