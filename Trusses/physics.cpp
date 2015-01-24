//
//  physics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "physics.h"

#include "particle.h"
#include "wall.h"
#include "bar.h"

#include <sys/time.h>

unsigned long long int t = 0.0;
unsigned long long int prev_t = 0.0;
double delta_t = 0.0;
double g = 10.0;
double energy_absorption = 0.5;

#define MAX_STRAIN 0.5
#define SMALL_NUM 1e-5

bool gravity = false;

void microsecond_time (unsigned long long &t)
// Returns system time in microseconds, used for introducing delays at low simulation speeds
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    t = (unsigned long long)tv.tv_usec + 1000000 * (unsigned long long)tv.tv_sec;
}

void update_time()
{
    prev_t = t;
    microsecond_time(t);
    delta_t = (t - prev_t)/1000000.0;
}

void check_boundaries(Particle& p)
{
    double eng_regained_perp = 1 - energy_absorption;
    double eng_regained_par = 1 - 0.1*energy_absorption;
    
    Vector2d vel = p.velocity_;
    Vector2d pos = p.position_;
    Vector2d prev_pos = p.prev_position_;
    
    SlotMap<Wall>::iterator walls_it;
    for (walls_it = walls.begin(); walls_it != walls.end(); walls_it++)
    {
        double wall_top = walls_it->y_max();
        double wall_bottom = walls_it->y_min();
        double wall_left = walls_it->x_min();
        double wall_right = walls_it->x_max();
        
        // Particle penetrated the bottom wall
        if (prev_pos.y <= wall_bottom && pos.y >= wall_bottom && prev_pos.x >= wall_left && prev_pos.x <= wall_right)
        {
            p.position_.y = wall_bottom;
            p.velocity_ = Vector2d(eng_regained_par*vel.x, -eng_regained_perp*vel.y);
            
            // So it doesn't break the Verlet integrator
            p.prev_position_verlet_ = p.position_ - delta_t * p.velocity_;
        }
        
        // Particle penetrated the left wall
        else if (prev_pos.x <= wall_left && pos.x >= wall_left && prev_pos.y <= wall_top && prev_pos.y >= wall_bottom)
        {
            p.position_.x = wall_left;
            p.velocity_ = Vector2d(-eng_regained_perp*vel.x, eng_regained_par*vel.y);
            p.prev_position_verlet_ = p.position_ - delta_t * p.velocity_;
        }
        
        // Particle penetrated the right wall
        else if (prev_pos.x >= wall_right && pos.x <= wall_right && prev_pos.y <= wall_top && prev_pos.y >= wall_bottom)
        {
            p.position_.x = wall_right;
            p.velocity_ = Vector2d(-eng_regained_perp*vel.x, eng_regained_par*vel.y);
            p.prev_position_verlet_ = p.position_ - delta_t * p.velocity_;
        }
        
        // Particle penetrated the top wall
        else if (prev_pos.y >= wall_top && pos.y <= wall_top && prev_pos.x >= wall_left && prev_pos.x <= wall_right)
        {
            p.position_.y = wall_top;
            p.velocity_ = Vector2d(eng_regained_par*vel.x, -eng_regained_perp*vel.y);
            p.prev_position_verlet_ = p.position_ - delta_t * p.velocity_;
        }
    }
}

void integrate(Particle& p, Vector2d acc, double dt)
{
    // verlet
    Vector2d next_position = 2 * p.position_ - p.prev_position_verlet_ + pow(delta_t, 2) * acc;
    p.velocity_ = (0.5 / delta_t) * (next_position - p.prev_position_);
    p.prev_position_verlet_ = p.position_;
    p.position_ = next_position;
}

void update_position()
{
    // Zero the acceleration and remember the previous position
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
    {
        if (!particles_it->fixed_)
        {
            particles_it->acceleration_ = Vector2d(0.0, 0.0);
            particles_it->prev_position_ = particles_it->position_;
            
            if (particles_it->trace_on)
                particles_it->trace.add(particles_it->position_);
        }
    }
    
    // Increase this to increase the stiffness
    int iterations = 30;
    for (int j = 0; j < iterations; j++)
    {
        SlotMap<Bar>::iterator bars_it;
        for (bars_it = bars.begin(); bars_it != bars.end(); bars_it++)
        {
            int particle_location(int id);
            Particle* p1 = &particles[bars_it->p1_id];
            Particle* p2 = &particles[bars_it->p2_id];
            
            double extension = bars_it->extension();

            double im1 = 1/p1->mass_;
            double im2 = 1/p1->mass_;
            float mult1 = (im1 / (im1 + im2)) * bars_it->stiffness;
            float mult2 = bars_it->stiffness - mult1;
            
            if (!p1->fixed_)
                p1->position_ += mult1 * extension * bars_it->unit21();
            if (!p2->fixed_)
                p2->position_ += mult2 * extension * bars_it->unit12();
        }
    }
    
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
    {
        if (particles_it->fixed_)
        {
            if (particles_it->oscillate)
                particles_it->move();
        }
        else
        {
            particles_it->acceleration_ += particles_it->external_acceleration_;
            
            if (gravity)
                particles_it->acceleration_ += Vector2d(0.0, -g);
            
            integrate(*particles_it, particles_it->acceleration_, delta_t);
            
            // Bounce the particles off the edges
            check_boundaries(*particles_it);
        }
    }
    
    // Destroy bars which are extended by too much
    SlotMap<Bar>::iterator bars_it;
    for (bars_it = bars.begin(); bars_it != bars.end(); bars_it++)
    {
        double extension = bars_it->extension() / bars_it->r0;
        if (extension > MAX_STRAIN || extension < -MAX_STRAIN)
        {
            Bar::destroy(bars_it->id_);
            bars_it--; // The order of bars was mixed up. The last one ended up here, so
                 // we need to recheck the ith bar 
        }
    }
}
