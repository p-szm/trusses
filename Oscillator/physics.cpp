//
//  physics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "physics.h"

#include <sys/time.h>

unsigned long long int t = 0.0;
unsigned long long int prev_t = 0.0;
double delta_t = 0.0;
double g = 10.0;
double spring_energy = 0.0;
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
    return 0.0;
    
    if (particles_number == 0)
        return 0.0;
    
    double total_energy;
    for (int i = 0; i < particles_number; i++)
    {
        double kinetic_energy = 0.5 * particles[i].mass * particles[i].velocity.abs2();
        double potential_energy = (gravity) ? particles[i].mass * g * (particles[i].position.y + 1.0) : 0.0;
        
        total_energy += kinetic_energy + potential_energy;
    }
    total_energy += spring_energy;
    
    return total_energy;
}

void check_boundaries(Particle& p)
{
    double eng_regained_perp = 1 - energy_absorption;
    double eng_regained_par = 1 - 0.1*energy_absorption;
    
    Vector2d vel = p.velocity;
    Vector2d pos = p.position;
    Vector2d prev_pos = p.prev_position;
    
    for (int i = 0; i < walls_number; i++)
    {
        Wall* w = &walls[i];
        double wall_top = w->centre_.y + w->height_/2.0;
        double wall_bottom = w->centre_.y - w->height_/2.0;
        double wall_left = w->centre_.x - w->width_/2.0;
        double wall_right = w->centre_.x + w->width_/2.0;
        
        // Particle penetrated the bottom wall
        if (prev_pos.y <= wall_bottom && pos.y >= wall_bottom && prev_pos.x >= wall_left && prev_pos.x <= wall_right)
        {
            p.position.y = wall_bottom;
            p.velocity = Vector2d(eng_regained_par*vel.x, -eng_regained_perp*vel.y);
            
            // So it doesn't break the Verlet integrator
            p.prev_position_verlet = p.position - delta_t * p.velocity;
        }
        
        // Particle penetrated the left wall
        else if (prev_pos.x <= wall_left && pos.x >= wall_left && prev_pos.y <= wall_top && prev_pos.y >= wall_bottom)
        {
            p.position.x = wall_left;
            p.velocity = Vector2d(-eng_regained_perp*vel.x, eng_regained_par*vel.y);
            p.prev_position_verlet = p.position - delta_t * p.velocity;
        }
        
        // Particle penetrated the right wall
        else if (prev_pos.x >= wall_right && pos.x <= wall_right && prev_pos.y <= wall_top && prev_pos.y >= wall_bottom)
        {
            p.position.x = wall_right;
            p.velocity = Vector2d(-eng_regained_perp*vel.x, eng_regained_par*vel.y);
            p.prev_position_verlet = p.position - delta_t * p.velocity;
        }
        
        // Particle penetrated the top wall
        else if (prev_pos.y >= wall_top && pos.y <= wall_top && prev_pos.x >= wall_left && prev_pos.x <= wall_right)
        {
            p.position.y = wall_top;
            p.velocity = Vector2d(eng_regained_par*vel.x, -eng_regained_perp*vel.y);
            p.prev_position_verlet = p.position - delta_t * p.velocity;
        }
    }
}

void integrate(Particle& p, Vector2d acc, double dt)
{
    if (euler)
    {
        p.velocity = p.velocity + dt * acc;
        p.position.x += delta_t * p.velocity.x;
        p.position.y += delta_t * p.velocity.y;
    }
    else // verlet
    {
        Vector2d next_position = 2 * p.position - p.prev_position_verlet + pow(delta_t, 2) * acc;
        p.velocity = (0.5 / delta_t) * (next_position - p.prev_position);
        p.prev_position_verlet = p.position;
        p.position = next_position;
    }
}

void update_position(std::vector<Particle>& particles)
{
    // Zero the acceleration and remember the previous position
    for (int i = 0; i < particles_number; i++)
    {
        particles[i].acceleration = Vector2d(0.0, 0.0);
        particles[i].prev_position = particles[i].position;
    }
    
    // Increase this to increase the stiffness
    int iterations = 50;
    for (int j = 0; j < iterations; j++)
    {
        for (int i = 0; i < bars_number; i++)
        {
            Bar* b = &bars[i];
            Particle* p1 = &particles[b->p1_id];
            Particle* p2 = &particles[b->p2_id];
            
            double extension = b->length() - b->r0;
            
            double k = 1.0; // max is 1.0 or it blows up
            double im1 = 1/p1->mass;
            double im2 = 1/p1->mass;
            float mult1 = (im1 / (im1 + im2)) * k;
            float mult2 = k - mult1;
            
            if (!p1->fixed)
                p1->position += mult1 * extension * b->unit21();
            if (!p2->fixed)
                p2->position += mult2 * extension * b->unit12();
        }
    }
    
    for (int i = 0; i < particles_number; i++)
    {
        Particle* p = &particles[i];
        
        if (p->fixed)
            continue;
        
        p->acceleration += p->external_acceleration;
        
        if (gravity)
            p->acceleration += Vector2d(0.0, -g);
        
        integrate(*p, p->acceleration, delta_t);
        
        // Bounce the particles off the edges
        check_boundaries(*p);
    }
}
