//
//  particle.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "particle.h"

float random(float range)
{
    float r = 1.0 * rand() / RAND_MAX;
    return r * 2 * range - range;
}

Particle Particle::create(double a, double b, bool fixed)
{
    Particle p(a, b);
    p.fixed = (fixed) ? true : false;
    
    Vector2d velocity = (fixed) ? Vector2d(0.0, 0.0) : Vector2d(random(MAX_VELOCITY), random(MAX_VELOCITY));
    p.velocity = velocity;
    
    p.prev_position = p.position - delta_t * p.velocity;
    
    p.acceleration = Vector2d(0.0, 0.0);
    
    p.mass = 10.0;
    if (particles.size() == 0)
    {
        p.r01 = 0.0;
        p.r02 = 0.0;
    }
    if (particles.size() == 1)
    {
        double r = (p.position - particles[particles.size()-1].position).abs();
        p.r01 = r;
        p.r02 = r;
        particles[0].r01 = r;
        particles[0].r02 = r;
    }
    else if (particles.size() > 1)
    {
        p.r01 = (p.position - particles[particles.size()-1].position).abs();
        p.r02 = (p.position - particles[0].position).abs();
        particles[particles.size()-1].r02 = p.r01;
        particles[0].r01 = p.r02;
    }
    
    p.highlight = false;
    
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