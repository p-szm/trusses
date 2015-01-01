//
//  physics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "physics.h"
#include "graphics.h"

unsigned long long int t = 0.0;
unsigned long long int prev_t = 0.0;
double delta_t = 0.0;
double g = 1.0;
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
        double wall_top = w->centre.y + w->height/2.0;
        double wall_bottom = w->centre.y - w->height/2.0;
        double wall_left = w->centre.x - w->width/2.0;
        double wall_right = w->centre.x + w->width/2.0;
        
        // If the particle entered the wall
        if (pos.x >= wall_left && pos.x <= wall_right && pos.y >= wall_bottom && pos.y <= wall_top)
        {
            
            
            // Particle penetrated the bottom wall
            if (prev_pos.y <= wall_bottom && pos.y >= wall_bottom)
            {
                p.position.y = wall_bottom;
                p.velocity = Vector2d(eng_regained_par*vel.x, -eng_regained_perp*vel.y);
            }
            
            // Particle penetrated the left wall
            else if (prev_pos.x <= wall_left && pos.x >= wall_left)
            {
                p.position.x = wall_left;
                p.velocity = Vector2d(-eng_regained_perp*vel.x, eng_regained_par*vel.y);
            }
            
            // Particle penetrated the right wall
            else if (prev_pos.x >= wall_right && pos.x <= wall_right)
            {
                p.position.x = wall_right;
                p.velocity = Vector2d(-eng_regained_perp*vel.x, eng_regained_par*vel.y);
            }
            
            // Particle penetrated the top wall
            else
            {
                p.position.y = wall_top;
                p.velocity = Vector2d(eng_regained_par*vel.x, -eng_regained_perp*vel.y);
            }
            
            // So it doesn't break the Verlet integrator
            p.prev_position = p.position - delta_t * p.velocity;
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
        Vector2d next_position = 2 * p.position - p.prev_position + pow(delta_t, 2) * acc;
        p.velocity = (0.5 / delta_t) * (next_position - p.prev_position); // one step behind the position
        p.prev_position = p.position;
        p.position = next_position;
    }
}

void update_position(std::vector<Particle>& particles)
{
    // Zero the acceleration
    for (int i = 0; i < particles_number; i++)
    {
        particles[i].acceleration = Vector2d(0.0, 0.0);
    }
    
    for (int i = 0; i < bars_number; i++)
    {
        Vector2d force = (bars[i].tension() + bars[i].extension_rate() * bars[i].lambda) * bars[i].unit21();
        particles[bars[i].p1_id].acceleration += (1.0/particles[bars[i].p1_id].mass) * force;
        particles[bars[i].p2_id].acceleration += -(1.0/particles[bars[i].p2_id].mass) * force;
    }
    
    for (int i = 0; i < particles_number; i++)
    {
        if (particles[i].fixed)
            continue;
        
        particles[i].acceleration += particles[i].external_acceleration;
        
        if (gravity)
            particles[i].acceleration = Vector2d(particles[i].acceleration.x, particles[i].acceleration.y - g);
        
        integrate(particles[i], particles[i].acceleration, delta_t);
        
        // Bounce the particles off the edges
        check_boundaries(particles[i]);
    }
}
