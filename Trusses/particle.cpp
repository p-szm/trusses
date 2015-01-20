//
//  particle.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "particle.h"
#include "physics.h"

std::vector<Particle> particles;
std::vector<int> particles_slots;
std::vector<ID> free_particles_id;

int particles_number = 0;

// Returns the next position
Vector2d right(Vector2d pos)
{
    double speed = 1.0;
    return (Vector2d(pos.x + speed * delta_t, pos.y));
}

// Moves the particle
void Particle::move()
{
    double new_phase = oscillation_.phase + delta_t*oscillation_.frequency*2*M_PI;
    oscillation_.phase = new_phase;
    
    if (oscil_dir == X)
    {
        position_.x = oscillation_.origin.x + oscillation_.amplitude * sin(oscillation_.phase);
    }
    else if (oscil_dir == Y)
    {
        position_.y = oscillation_.origin.y + oscillation_.amplitude * sin(oscillation_.phase);
    }
    else if (oscil_dir == CW)
    {
        position_.x = oscillation_.origin.x + oscillation_.amplitude * cos(oscillation_.phase);
        position_.y = oscillation_.origin.y + oscillation_.amplitude * sin(oscillation_.phase);
    }
    else if (oscil_dir == ACW)
    {
        position_.x = oscillation_.origin.x + oscillation_.amplitude * cos(oscillation_.phase);
        position_.y = oscillation_.origin.y + oscillation_.amplitude * sin(-oscillation_.phase);
    }
}

ID Particle::create(double a, double b, bool fixed)
{
    // TODO
    // Check if a particle of this id already exists

    particles_number++;
    Particle p(a, b);
    ID new_id;
    
    p.velocity_ = Vector2d(0.0, 0.0);
    p.prev_position_ = p.position_ - delta_t * p.velocity_;
    p.prev_position_verlet_ = p.position_ - delta_t * p.velocity_;
    p.acceleration_ = Vector2d(0.0, 0.0);
    p.external_acceleration_ = Vector2d(0.0, 0.0);
    p.mass_ = 1.0;
    
    p.fixed_ = (fixed) ? true : false;
    p.dragged_ = false;
    
    // Assume that the particle is fixed
    p.oscil_dir = CW;
    p.oscillation_.phase = 0.0;
    p.oscillation_ = Oscillation(p.position_, 0.1, 1.5);
    
    // If there are some spare ids
    if (!free_particles_id.empty())
    {
        // Grab the last one and pop it
        ID free_id = free_particles_id.back();
        free_particles_id.pop_back();
        p.id_ = free_id;
        
        particles.push_back(p);
        new_id = free_id;
    }
    
    // No free ids. Create a new one
    else
    {
        ID next_id = ID((int)particles.size(), 0);
        p.id_ = next_id;
        particles_slots.push_back(next_id.number);
        
        particles.push_back(p);
        new_id = next_id;
    }
    
    return new_id;
}