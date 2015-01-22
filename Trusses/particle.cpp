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

std::vector<Particle> particles;
std::vector<int> particles_slots;
std::vector<int> free_particles_id;

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

int Particle::create(double a, double b, bool fixed)
{
    particles_number++;
    Particle p(a, b);
    int new_id;
    
    p.velocity_ = Vector2d(0.0, 0.0);
    p.prev_position_ = p.position_ - delta_t * p.velocity_;
    p.prev_position_verlet_ = p.position_ - delta_t * p.velocity_;
    p.acceleration_ = Vector2d(0.0, 0.0);
    p.external_acceleration_ = Vector2d(0.0, 0.0);
    p.mass_ = 1.0;
    
    p.fixed_ = (fixed) ? true : false;
    p.dragged_ = false;
    
    // Assume that the particle is fixed
    p.oscil_dir = X;
    p.oscillation_.phase = 0.0;
    p.oscillation_ = Oscillation(p.position_, 0.1, 4);
    
    // If there are some spare ids
    if (!free_particles_id.empty())
    {
        // Grab the last one and pop it
        int free_id = free_particles_id.back();
        free_particles_id.pop_back();
        p.id_ = free_id;
        
        particles.push_back(p);
        new_id = free_id;
    }
    
    // No free ids. Create a new one
    else
    {
        int next_id = (int)particles.size();
        p.id_ = next_id;
        particles_slots.push_back(next_id);
        
        particles.push_back(p);
        new_id = next_id;
    }
    
    return new_id;
}

void Particle::destroy(int removed_id)
{
    // Check if this particle exists
    bool numbers_agree = (removed_id < particles_slots.size() && particles_slots[removed_id] != -1) ? true : false;
    if (!numbers_agree)
    {
        std::cout << "Particle " << removed_id << " doesn't exist" << std::endl;
        return;
    }
    
    int position_of_removed_particle = particles_slots[removed_id];
    int swapped_particle_id = particles.back().id_;
    
    // Remove all the bars that this particle was connected to
    Particle* this_p = &particles[particle_location(removed_id)];
    while (this_p->bars_connected.size() > 0)
        Bar::destroy(this_p->bars_connected.back());
    
    // Swap the particle being destroyed with the last one
    Particle last_copy = particles.back();
    particles.back() = particles[position_of_removed_particle];
    particles[position_of_removed_particle] = last_copy;
    
    // Pop the last particle
    particles.pop_back();
    
    // Update the slots
    particles_slots[swapped_particle_id] = position_of_removed_particle;
    particles_slots[removed_id] = -1; // This particle doesn't exist
    
    // Update the free list
    free_particles_id.push_back(removed_id);
    
    particles_number--;
}

int particle_location(int id)
{
    // size_t is unsigned, so be careful
    // If the id has never existed or it is not used now
    if (id + 1 > (int)particles_slots.size() || particles_slots[id] == -1)
        return -1;
    else
        return particles_slots[id];
}

void print_particles()
{
    for (int i = 0; i < particles.size(); i++)
    {
        //std::cout << "Particle (" << particles[i].id_.number << " " << particles[i].id_.version << ')' << std::endl;
        std::cout << "Particle " << particles[i].id_ << ": ";
        for (int j = 0; j < particles[i].bars_connected.size(); j++)
        {
            std::cout << particles[i].bars_connected[j] << " ";
        }
        std::cout << std::endl;
    }
}

void reset_particles()
{
    particles.clear();
    particles_slots.clear();
    free_particles_id.clear();
    particles_number = 0;
}