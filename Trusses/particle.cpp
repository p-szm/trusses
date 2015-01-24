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

SlotMap<Particle> particles;

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
    
    // Assume that the particle is fixed
    new_particle.oscil_dir = X;
    new_particle.oscillation_.phase = 0.0;
    new_particle.oscillation_ = Oscillation(new_particle.position_, 0.1, 4);
    new_particle.oscillate = false;
    
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