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

bool euler = false;
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

double energy(std::vector<Particle>& particles)
{
    if (particles_number == 0)
        return 0.0;
    
    double total_energy;
    for (int i = 0; i < particles_number; i++)
    {
        double kinetic_energy = 0.5 * particles[i].mass_ * particles[i].velocity_.abs2();
        double potential_energy = (gravity) ? particles[i].mass_ * g * (particles[i].position_.y + 1.0) : 0.0;
        
        total_energy += kinetic_energy;// + potential_energy;
    }
    
    return total_energy;
}

void check_boundaries(Particle& p)
{
    double eng_regained_perp = 1 - energy_absorption;
    double eng_regained_par = 1 - 0.1*energy_absorption;
    
    Vector2d vel = p.velocity_;
    Vector2d pos = p.position_;
    Vector2d prev_pos = p.prev_position_;
    
    for (int i = 0; i < walls_number; i++)
    {
        Wall* w = &walls[i];
        double wall_top = w->y_max();
        double wall_bottom = w->y_min();
        double wall_left = w->x_min();
        double wall_right = w->x_max();
        
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
    if (euler)
    {
        p.velocity_ = p.velocity_ + dt * acc;
        p.position_.x += delta_t * p.velocity_.x;
        p.position_.y += delta_t * p.velocity_.y;
    }
    else // verlet
    {
        Vector2d next_position = 2 * p.position_ - p.prev_position_verlet_ + pow(delta_t, 2) * acc;
        p.velocity_ = (0.5 / delta_t) * (next_position - p.prev_position_);
        p.prev_position_verlet_ = p.position_;
        p.position_ = next_position;
    }
}

void update_position(std::vector<Particle>& particles)
{
    // Zero the acceleration and remember the previous position
    for (int i = 0; i < particles_number; i++)
    {
        particles[i].acceleration_ = Vector2d(0.0, 0.0);
        particles[i].prev_position_ = particles[i].position_;
    }
    
    // Increase this to increase the stiffness
    int iterations = 30;
    for (int j = 0; j < iterations; j++)
    {
        for (int i = 0; i < bars_number; i++)
        {
            Bar* b = &bars[i];
            Particle* p1 = &particles[b->p1_id];
            Particle* p2 = &particles[b->p2_id];
            
            double extension = b->length() - b->r0;
            
            double k = 0.5; // max is 1.0 or it blows up
            double im1 = 1/p1->mass_;
            double im2 = 1/p1->mass_;
            float mult1 = (im1 / (im1 + im2)) * k;
            float mult2 = k - mult1;
            
            if (!p1->fixed_)
                p1->position_ += mult1 * extension * b->unit21();
            if (!p2->fixed_)
                p2->position_ += mult2 * extension * b->unit12();
        }
    }
    
    for (int i = 0; i < particles_number; i++)
    {
        Particle* p = &particles[i];
        
        if (p->fixed_)
            continue;
        
        p->acceleration_ += p->external_acceleration_;
        
        if (gravity)
            p->acceleration_ += Vector2d(0.0, -g);
        
        integrate(*p, p->acceleration_, delta_t);
        
        // Bounce the particles off the edges
        check_boundaries(*p);
    }
}
