//
//  bar.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "bar.h"
#include "particle.h"

SlotMap<Bar> bars;

int Bar::create(int id1, int id2)
{
    // TODO
    // Check if this bar already exists (connecting two particles)
    
    if (id1 == id2)
    {
        std::cout << "Canot create a bar connecting the same particle" << std::endl;
        return -1;
    }
    
    
    if (!particles.exists(id1) || !particles.exists(id2))
    {
        std::cout << "One or more required particles don't exist" << std::endl;
        return -1;
    }
    
    Bar new_bar(id1, id2);
    new_bar.r0 = new_bar.length();
    int new_id = bars.add(new_bar);
    
    // Particles have to know which bars are connected to them
    particles[id1].bars_connected.push_back(new_id);
    particles[id2].bars_connected.push_back(new_id);

    return new_id;
}

int Bar::destroy(int obj_id)
{
    if (!bars.exists(obj_id))
    {
        std::cout << "This bar doesn't exist" << std::endl;
        return 1;
    }
    
    Particle* p1 = &particles[bars[obj_id].p1_id];
    Particle* p2 = &particles[bars[obj_id].p2_id];
    
    for (int i = 0; i < p1->bars_connected.size(); i++)
    {
        if (p1->bars_connected[i] == obj_id)
        {
            p1->bars_connected[i] = p1->bars_connected.back();
            p1->bars_connected.pop_back();
        }
    }
    for (int i = 0; i < p2->bars_connected.size(); i++)
    {
        if (p2->bars_connected[i] == obj_id)
        {
            p2->bars_connected[i] = p2->bars_connected.back();
            p2->bars_connected.pop_back();
        }
    }
    
    bars.remove(obj_id);
    
    return 0;
}

double Bar::length() const
{
    Vector2d pos1 = particles[p1_id].position_;
    Vector2d pos2 =particles[p2_id].position_;
    double ans = (pos1 - pos2).abs();
    return ans;
}

Vector2d Bar::unit12() const
{
    return (particles[p1_id].position_ - particles[p2_id].position_).norm();
}

Vector2d Bar::unit21() const
{
    return -unit12();
}

double Bar::extension() const
{
    double len = length();
    double r_not = r0;
    double ans = len - r_not;
    return ans;
}

void reset_bars()
{
    bars.clear();
}

void print_bars()
{
    SlotMap<Bar>::iterator bars_it;
    for (bars_it = bars.begin(); bars_it != bars.end(); bars_it++)
    {
        std::cout << "Bar " << bars_it->id_ << std::endl;
    }
}