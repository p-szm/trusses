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
    if (particles.size() == 0)
        return 0.0;
    
    double total_energy;
    for (int i = 0; i < particles.size(); i++)
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
    float k = 10.0;
    float lambda = 10.0;
    //float r0 = 0.2;
    
    spring_energy = 0.0;
    
    size_t no_points = particles.size();
    
    for (int i = 0; i < no_points; i++)
    {
        if (particles[i].fixed || no_points == 1)
            continue;
        
        Particle prev_p = (i > 0) ? particles[i-1] : particles[no_points-1];
        Particle curr_p = particles[i];
        Particle next_p = (i < no_points-1) ? particles[i+1] : particles[0];
        
        Vector2d r1 = curr_p.position - prev_p.position;
        Vector2d r2 = curr_p.position - next_p.position;
        
        Vector2d unit_r1 = r1.norm();
        Vector2d unit_r2 = r2.norm();
        
        Vector2d force1 = -k * (r1 - particles[i].r01 * unit_r1) - lambda * (curr_p.velocity * r1) * unit_r1;
        Vector2d force2 = -k * (r2 - particles[i].r02 * unit_r2) - lambda * (curr_p.velocity * r2) * unit_r2;
        
        particles[i].acceleration = (1.0/particles[i].mass)*(force1 + force2);
        
        if (no_points == 2)
        {
            // There is only one link
            particles[i].acceleration = 0.5 * particles[i].acceleration;
            spring_energy += 0.25 * k * (r1 - particles[i].r01 * unit_r1).abs2();
        }
        else
            spring_energy += 0.5 * k * (r1 - particles[i].r02 * unit_r1).abs2();
        
        if (gravity)
            particles[i].acceleration = Vector2d(particles[i].acceleration.x, particles[i].acceleration.y - g);
        
        integrate(particles[i], particles[i].acceleration, delta_t);
        
        // Bounce the particles off the edges
        check_boundaries(particles[i]);
    }
}
