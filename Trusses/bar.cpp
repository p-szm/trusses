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
std::vector<ID> free_bars_id;
int bars_number = 0;

ID Bar::create(int id1, int id2)
{
    // TODO
    // Check if this bar already exists (connecting two particles)
    
    if (id1 == id2)
    {
        // TODO
        std::cout << "Canot create a bar connecting the same particle" << std::endl;
        return ID(-1, -1);
    }
    
    bars_number++;
    ID new_id;
    if (!free_bars_id.empty())
    {
        ID free_id = free_bars_id.back();
        free_bars_id.pop_back();
        
        bars.push_back(Bar(id1, id2));
        bars.back().id_ = free_id;
        
        /////
        bars.back().r0 = bars.back().length();
        
        bars_slots[free_id.number] = (int)bars.size() - 1;
        
        new_id = free_id;
    }
    else
    {
        ID next_id = ID((int)bars.size(), 0);
        bars.push_back(Bar(id1, id2));
        bars.back().id_ = next_id;
        
        /////
        bars.back().r0 = bars.back().length();
        
        bars_slots.push_back(next_id.number);
        new_id = next_id;
    }
    
    //std::cout << "Bar (" << new_id.number << " " << new_id.version << ") was created" << std::endl;

    return new_id;
}

void Bar::destroy(ID removed_id)
{
    // Check if this bar exists
    bool numbers_agree = (removed_id.number < bars_slots.size() && bars_slots[removed_id.number] != -1) ? true : false;
    if (!numbers_agree)
    {
        std::cout << "Bar (" << removed_id.number << ", " << removed_id.version << ") doesn't exist" << std::endl;
        return;
    }
    
    bool versions_agree = (removed_id.version == get_current_version(removed_id.number)) ? true : false;
    if (!versions_agree)
    {
        std::cout << "Bar (" << removed_id.number << ", " << removed_id.version << ") doesn't exist" << std::endl;
        return;
    }
    
    int swapped_bar_id = bars.back().id_.number;
    int position_of_removed_bar = bars_slots[removed_id.number];
    
    // Swap the bar being destroyed with the last one
    Bar last_copy = bars.back();
    bars.back() = bars[bars_slots[removed_id.number]];
    bars[position_of_removed_bar] = last_copy;
    
    // Pop the last bar
    bars.pop_back();
    
    // Update the slots
    bars_slots[swapped_bar_id] = position_of_removed_bar;
    bars_slots[removed_id.number] = -1; // This bar doesn't exist
    
    // Update the free list
    free_bars_id.push_back(removed_id);
    free_bars_id.back().version++; // Increment the version
    
    bars_number--;
    
    //std::cout << "Bar (" << removed_id.number << " " << removed_id.version << ") was destroyed" << std::endl;
}

int Bar::get_current_version(int number)
{
    return bars[bars_slots[number]].id_.version;
}

void reset_bars()
{
    bars.clear();
    bars_slots.clear();
    free_bars_id.clear();
    bars_number = 0;
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