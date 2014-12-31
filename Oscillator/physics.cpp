//
//  physics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "physics.h"

unsigned long long int t = 0.0;
unsigned long long int prev_t = 0.0;
double delta_t = 0.0;
double g = 1.0;
double spring_energy = 0.0;

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
    Vector2d vel = p.velocity;
    
    if (p.position.x >= 1.0)
    {
        p.position.x = 1.0;
        p.velocity = Vector2d(-0.9*vel.x, 0.99*vel.y);
        p.prev_position = p.position - delta_t * p.velocity;
    }
    
    else if (p.position.x <= -1.0)
    {
        p.position.x = -1.0;
        p.velocity = Vector2d(-0.9*vel.x, 0.99*vel.y);
        p.prev_position = p.position - delta_t * p.velocity;
    }
    
    if (p.position.y >= 1.0)
    {
        p.position.y = 1.0;
        p.velocity = Vector2d(0.99*vel.x, -0.9*vel.y);
        p.prev_position = p.position - delta_t * p.velocity;
    }
    
    else if (p.position.y <= -1.0)
    {
        p.position.y = -1.0;
        p.velocity = Vector2d(0.99*vel.x, -0.9*vel.y);
        p.prev_position = p.position - delta_t * p.velocity;
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
        Vector2d acc = (bars[i].tension() + bars[i].extension_rate() * bars[i].lambda) * bars[i].unit21();
        particles[bars[i].p1_id].acceleration += acc;
        particles[bars[i].p2_id].acceleration += -acc;
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
