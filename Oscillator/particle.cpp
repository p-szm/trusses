//
//  particle.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "particle.h"

int particles_number = 0;
int bars_number = 0;
int walls_number = 0;

std::vector<Particle> particles;
std::vector<Bar> bars;
std::vector<Wall> walls;

float random(float range)
{
    float r = 1.0 * rand() / RAND_MAX;
    return r * 2 * range - range;
}

Particle Particle::create(double a, double b, bool fixed)
{
    // TODO
    // Check if a particle of this id already exists
    
    Particle p(a, b);
    p.fixed = (fixed) ? true : false;
    p.mass = 1.0;
    
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
    // TODO
    // Check if this bar already exists (connecting two particles)
    // Check if a bar of this id already exists
    
    if (id1 == id2)
    {
        // TODO
        std::cout << "Canot create a bar connecting the same particle" << std::endl;
    }
    
    Bar b = Bar(id1, id2);
    b.r0 = b.length();
    
    b.id = bars_number;
    bars_number++;
    
    return b;
}

double Bar::length() const
{
    return (particles[p1_id].position - particles[p2_id].position).abs();
}

Vector2d Bar::unit12() const
{
    return (particles[p1_id].position - particles[p2_id].position).norm();
}

Vector2d Bar::unit21() const
{
    return -unit12();
}

double Bar::extension_rate() const
{
    return unit21() * (particles[p2_id].velocity - particles[p1_id].velocity);
}

/////

Wall Wall::create(Vector2d c, double w, double h)
{
    Wall wl = Wall(c, w, h);
    
    wl.id = walls_number;
    walls_number ++;
    
    return wl;
}