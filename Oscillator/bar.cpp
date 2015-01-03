//
//  bar.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "bar.h"
#include "particle.h"

std::vector<Bar> bars;
int bars_number = 0;

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
    return (particles[p1_id].position_ - particles[p2_id].position_).abs();
}

Vector2d Bar::unit12() const
{
    return (particles[p1_id].position_ - particles[p2_id].position_).norm();
}

Vector2d Bar::unit21() const
{
    return -unit12();
}

double Bar::extension_rate() const
{
    return unit21() * (particles[p2_id].velocity_ - particles[p1_id].velocity_);
}