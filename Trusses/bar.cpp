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
std::vector<int> bars_slots;
std::vector<int> free_bars_id;
int bars_number = 0;

int Bar::create(int id1, int id2)
{
    // TODO
    // Check if this bar already exists (connecting two particles)
    
    if (id1 == id2)
    {
        std::cout << "Canot create a bar connecting the same particle" << std::endl;
        return -1;
    }
    
    if (particle_location(id1) == -1 || particle_location(id2) == -1)
    {
        std::cout << "One or more required particles don't exist" << std::endl;
        return -1;
    }
    
    bars_number++;
    int new_id;
    if (!free_bars_id.empty())
    {
        int free_id = free_bars_id.back();
        free_bars_id.pop_back();
        
        bars.push_back(Bar(id1, id2));
        bars.back().id_ = free_id;
        
        /////
        bars.back().r0 = bars.back().length();
        
        bars_slots[free_id] = (int)bars.size() - 1;
        
        new_id = free_id;
    }
    else
    {
        int next_id = (int)bars.size();
        bars.push_back(Bar(id1, id2));
        bars.back().id_ = next_id;
        
        /////
        bars.back().r0 = bars.back().length();
        
        bars_slots.push_back(next_id);
        new_id = next_id;
    }
    
    // Particles have to know which bars are connected to them
    particles[particle_location(id1)].bars_connected.push_back(new_id);
    particles[particle_location(id2)].bars_connected.push_back(new_id);

    return new_id;
}

void Bar::destroy(int removed_id)
{
    // Check if this bar exists
    bool numbers_agree = (removed_id < bars_slots.size() && bars_slots[removed_id] != -1) ? true : false;
    if (!numbers_agree)
    {
        std::cout << "Bar " << removed_id << " doesn't exist" << std::endl;
        return;
    }
    
    int swapped_bar_id = bars.back().id_;
    int position_of_removed_bar = bars_slots[removed_id];
    
    // "Disconnect" bars from particles
    int p1_id = bars[position_of_removed_bar].p1_id;
    int p2_id = bars[position_of_removed_bar].p2_id;
    Particle* p1 = &particles[particle_location(p1_id)];
    Particle* p2 = &particles[particle_location(p2_id)];
    for (int i = 0; i < p1->bars_connected.size(); i++)
    {
        if (p1->bars_connected[i] == removed_id)
        {
            p1->bars_connected[i] = p1->bars_connected.back();
            p1->bars_connected.pop_back();
        }
    }
    for (int i = 0; i < p2->bars_connected.size(); i++)
    {
        if (p2->bars_connected[i] == removed_id)
        {
            p2->bars_connected[i] = p2->bars_connected.back();
            p2->bars_connected.pop_back();
        }
    }
    
    // Swap the bar being destroyed with the last one
    Bar last_copy = bars.back();
    bars.back() = bars[bars_slots[removed_id]];
    bars[position_of_removed_bar] = last_copy;
    
    // Pop the last bar
    bars.pop_back();
    
    // Update the slots
    bars_slots[swapped_bar_id] = position_of_removed_bar;
    bars_slots[removed_id] = -1; // This bar doesn't exist
    
    // Update the free list
    free_bars_id.push_back(removed_id);
    
    bars_number--;
}

double Bar::length() const
{
    Vector2d pos1 = particles[particle_location(p1_id)].position_;
    Vector2d pos2 =particles[particle_location(p2_id)].position_;
    double ans = (pos1 - pos2).abs();
    return ans;
}

Vector2d Bar::unit12() const
{
    return (particles[particle_location(p1_id)].position_ - particles[particle_location(p2_id)].position_).norm();
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
    bars_slots.clear();
    free_bars_id.clear();
    bars_number = 0;
}

void print_bars()
{
    for (int i = 0; i < bars.size(); i++)
    {
        std::cout << "Bar " << bars[i].id_ << std::endl;
    }
}

int bar_location(int id)
{
    // size_t is unsigned, so be careful
    // If the id has never existed or it is not used now
    if (id + 1 > (int)bars_slots.size() || bars_slots[id] == -1)
        return -1;
    else
        return bars_slots[id];
}