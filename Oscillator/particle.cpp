//
//  particle.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "particle.h"

int selected_particle_id = -1;
int particles_number = 0;
int bars_number = 0;

float random(float range)
{
    float r = 1.0 * rand() / RAND_MAX;
    return r * 2 * range - range;
}

Particle Particle::create(double a, double b, bool fixed)
{
    Particle p(a, b);
    p.fixed = (fixed) ? true : false;
    p.mass = 10.0;
    
    //Vector2d velocity = (fixed) ? Vector2d(0.0, 0.0) : Vector2d(random(MAX_VELOCITY), random(MAX_VELOCITY));
    Vector2d velocity = Vector2d(0.0, 0.0);
    p.velocity = velocity;
    
    p.prev_position = p.position - delta_t * p.velocity;
    
    p.acceleration = Vector2d(0.0, 0.0);
    p.external_acceleration = Vector2d(0.0, 0.0);
    
    if (particles_number == 0)
    {
        p.r01 = 0.0;
        p.r02 = 0.0;
    }
    if (particles_number == 1)
    {
        double r = (p.position - particles[particles_number-1].position).abs();
        p.r01 = r;
        p.r02 = r;
        particles[0].r01 = r;
        particles[0].r02 = r;
    }
    else if (particles_number > 1)
    {
        p.r01 = (p.position - particles[particles_number-1].position).abs();
        p.r02 = (p.position - particles[0].position).abs();
        particles[particles_number-1].r02 = p.r01;
        particles[0].r01 = p.r02;
    }
    
    p.highlight = false;
    p.dragged = false;
    
    p.id = particles_number;
    particles_number++;
    
    return p;
}

void Particle::fix()
{
    fixed = true;
    velocity = Vector2d(0.0, 0.0);
    acceleration = Vector2d(0.0, 0.0);
}

void Particle::unfix()
{
    fixed = false;
}


////

Bar Bar::create(int id1, int id2)
{
    Bar b = Bar(id1, id2);
    
    b.k = 100.0;
    b.lambda = 10.0;
    b.r0 = b.length();
    
    b.id = particles_number;
    bars_number++;
    
    return b;
}

double Bar::length()
{
    return (particles[p1_id].position - particles[p2_id].position).abs();
}

Vector2d Bar::unit12()
{
    return (particles[p1_id].position - particles[p2_id].position).norm();
}

Vector2d Bar::unit21()
{
    return -unit12();
}

double Bar::extension_rate()
{
    return unit21() * (particles[p2_id].velocity - particles[p1_id].velocity);
}

double Bar::tension()
{
    return (length()-r0)*k;
}