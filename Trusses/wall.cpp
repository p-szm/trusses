//
//  wall.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "wall.h"

std::vector<Wall> walls;
std::vector<int> walls_slots;
std::vector<int> free_walls_id;

int walls_number = 0;

int Wall::create(Vector2d p1, Vector2d p2)
{
    walls_number ++;
    
    int new_id;
    if (!free_walls_id.empty())
    {
        int free_id = free_walls_id.back();
        free_walls_id.pop_back();
        
        walls.push_back(Wall(p1, p2));
        walls.back().id_ = free_id;
        
        walls_slots[free_id] = (int)walls.size() - 1;
        
        new_id = free_id;
    }
    else
    {
        int next_id = (int)walls.size();
        walls.push_back(Wall(p1, p2));
        walls.back().id_ = next_id;
        
        walls_slots.push_back(next_id);
        new_id = next_id;
    }
    
    return new_id;
}

void Wall::destroy(int removed_id)
{
    // Check if this wall exists
    bool numbers_agree = (removed_id < walls.size()) ? true : false;
    if (!numbers_agree)
    {
        std::cout << "Wall " << removed_id << " doesn't exist" << std::endl;
        return;
    }
    
    int swapped_wall_id = walls.back().id_;
    int position_of_removed_wall = walls_slots[removed_id];
    
    // Swap the wall being destroyed with the last one
    Wall last_copy = walls.back();
    walls.back() = walls[walls_slots[removed_id]];
    walls[position_of_removed_wall] = last_copy;
    
    // Pop the last wall
    walls.pop_back();
    
    // Update the slots
    walls_slots[swapped_wall_id] = position_of_removed_wall;
    walls_slots[removed_id] = -1; // This wall doesn't exist
    
    // Update the free list
    free_walls_id.push_back(removed_id);
    
    walls_number--;
}

void reset_walls()
{
    walls.clear();
    walls_slots.clear();
    free_walls_id.clear();
    walls_number = 0;
}

double Wall::x_min() const
{
    return min(p1_.x, p2_.x);
}

double Wall::x_max() const
{
    return max(p1_.x, p2_.x);
}

double Wall::y_min() const
{
    return min(p1_.y, p2_.y);
}

double Wall::y_max() const
{
    return max(p1_.y, p2_.y);
}

void print_walls()
{
    for (int i = 0; i < walls.size(); i++)
    {
        std::cout << "Wall " << walls[i].id_ << std::endl;
    }
}

int wall_location(int id)
{
    // size_t is unsigned, so be careful
    // If the id has never existed or it is not used now
    if (id + 1 > (int)walls_slots.size() || walls_slots[id] == -1)
        return -1;
    else
        return walls_slots[id];
}