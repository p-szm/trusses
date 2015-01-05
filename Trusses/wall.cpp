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
std::vector<ID> free_walls_id;

int walls_number = 0;

ID Wall::create(Vector2d p1, Vector2d p2)
{
    walls_number ++;
    
    ID new_id;
    if (!free_walls_id.empty())
    {
        ID free_id = free_walls_id.back();
        free_walls_id.pop_back();
        
        walls.push_back(Wall(p1, p2));
        walls.back().id_ = free_id;
        
        walls_slots[free_id.number] = (int)walls.size() - 1;
        
        new_id = free_id;
    }
    else
    {
        ID next_id = ID((int)walls.size(), 0);
        walls.push_back(Wall(p1, p2));
        walls.back().id_ = next_id;
        
        walls_slots.push_back(next_id.number);
        new_id = next_id;
    }
    
    /*
    std::cout << "Created a wall " << new_id.number << std::endl;
    std::cout << "walls: ";
    for (int i = 0; i < walls.size(); i++)
    {
        std::cout << '(' << walls[i].id_.number << ' ' << walls[i].id_.version << ')' << ' ';
    }
    std::cout << std::endl;
    
    std::cout << "walls_slots: ";
    for (int i = 0; i < walls_slots.size(); i++)
    {
        std::cout << walls_slots[i] << ' ';
    }
    std::cout << std::endl;
    
    std::cout << "free_walls_id: ";
    for (int i = 0; i < free_walls_id.size(); i++)
    {
        std::cout << '(' << free_walls_id[i].number << ' ' << free_walls_id[i].version << ')' << ' ';
    }
    std::cout << std::endl;
    */
    
    return new_id;
}

void Wall::destroy(ID removed_id)
{
    // Check if this wall exists
    bool numbers_agree = (removed_id.number < walls.size()) ? true : false;
    if (!numbers_agree)
    {
        std::cout << "Wall (" << removed_id.number << ", " << removed_id.version << ") doesn't exist" << std::endl;
        return;
    }
    
    bool versions_agree = (removed_id.version == get_current_version(removed_id.number)) ? true : false;
    if (!versions_agree)
    {
        std::cout << "Wall (" << removed_id.number << ", " << removed_id.version << ") doesn't exist" << std::endl;
        return;
    }
    
    int swapped_wall_id = walls.back().id_.number;
    int position_of_removed_wall = walls_slots[removed_id.number];
    
    // Swap the wall being destroyed with the last one
    Wall last_copy = walls.back();
    walls.back() = walls[walls_slots[removed_id.number]];
    walls[position_of_removed_wall] = last_copy;
    
    // Pop the last wall
    walls.pop_back();
    
    // Update the slots
    walls_slots[swapped_wall_id] = position_of_removed_wall;
    walls_slots[removed_id.number] = -1; // This wall doesn't exist
    
    // Update the free list
    free_walls_id.push_back(removed_id);
    free_walls_id.back().version++; // Increment the version
    
    walls_number--;
    
    /*
    std::cout << "Removed wall " << removed_id.number << std::endl;
    std::cout << "walls: ";
    for (int i = 0; i < walls.size(); i++)
    {
        std::cout << '(' << walls[i].id_.number << ' ' << walls[i].id_.version << ')' << ' ';
    }
    std::cout << std::endl;
    
    std::cout << "walls_slots: ";
    for (int i = 0; i < walls_slots.size(); i++)
    {
        std::cout << walls_slots[i] << ' ';
    }
    std::cout << std::endl;
    
    std::cout << "free_walls_id: ";
    for (int i = 0; i < free_walls_id.size(); i++)
    {
        std::cout << '(' << free_walls_id[i].number << ' ' << free_walls_id[i].version << ')' << ' ';
    }
    std::cout << std::endl;
     */
}

int Wall::get_current_version(int number)
{
    return walls[walls_slots[number]].id_.version;
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