//
//  save.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 01/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "save.h"

#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "particle.h"
#include "graphics.h"
#include "interface.h"
#include "interpreter.h"
#include "physics.h"

template<typename T>
void read_numbers(std::string str, std::vector<T> & target_v)
{
    // Convert the string to the stringstream
    std::istringstream s(str.substr(str.find(' ')+1, std::string::npos));
    
    // Read every "word" of a stringstream
    while (s)
    {
        T n;
        // If s is a number
        if (s >> n)
            target_v.push_back(n);
    }
}

int load(std::string filename)
{
    // TODO
    // Check if the file is valid
    
    // Important:
    // Sort out the problem with ids when loading
    
    // Important:
    // App freezes when loading walls
    // --- Fixed
    // But check the function read_numbers
    
    // Open the file
    std::ifstream file(filename);
    
    // Check if the file was opened successfully
    if (!file.is_open())
    {
        std::cerr << "File " << filename << " could not be opened" << std::endl;
        return 1;
    }
    
    std::cout << "File " << filename << " successfully opened" << std::endl;
    
    // Read the file line by line
    std::string line;
    while (getline(file, line))
    {
        // A particle
        if (line.substr(0, 1) == "p")
        {
            std::vector<double> v;
            read_numbers(line, v);
            
            if (v.size() == 2)
                Particle::create(v[0], v[1], false);
        }
        
        // A fixed particle
        else if (line.substr(0, 1) == "f")
        {
            std::vector<double> v;
            read_numbers(line, v);
            
            if (v.size() == 2)
                Particle::create(v[0], v[1], true);
        }
        
        // A bar
        else if (line.substr(0, 1) == "b")
        {
            std::vector<double> v;
            read_numbers(line, v);
            
            if (v.size() == 4)
                Bar::create((int)v[0], (int)v[1], v[2], v[3]);
            else if (v.size() == 2)
                Bar::create((int)v[0], (int)v[1], 0.0, ROOM_TEMPERATURE);
        }
        
        // A wall
        else if (line.substr(0, 1) == "w")
        {
            std::vector<int> v;
            read_numbers(line, v);
            
            if (v.size() == 4)
                Wall::create(Vector2d(v[0], v[1]), Vector2d(v[2], v[3]));
        }
    }
    
    // Close the file
    file.close();
    
    return 0;
}

void save(std::string filename)
{
    // Open the file
    std::ofstream file(filename);
    
    // Print time
    file << date_str() << ' ' << time_str() << std::endl << std::endl;
    
    // Print particles
    SlotMap<Particle>::iterator particles_it;
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
    {
        if (particles_it->fixed_)
            file << 'f';
        else
            file << 'p';
        file << particles_it->id_ << ' ' << particles_it->position_;
        file << std::endl;
    }
    file << std::endl;
    
    // Print bars
    // b-bar_id particle1_id particle2_id strain temperature
    SlotMap<Bar>::iterator bars_it;
    for (bars_it = bars.begin(); bars_it != bars.end(); bars_it++)
        file << 'b' << bars_it->id_ << ' ' << bars_it->p1_id << ' ' << bars_it->p2_id << ' ' << bars_it->get_strain() << ' ' << bars_it->get_temperature() << std::endl;
    file << std::endl;
    
    // Print walls
    SlotMap<Wall>::iterator walls_it;
    for (walls_it = walls.begin(); walls_it != walls.end(); walls_it++)
        file << 'w' << walls_it->id_ << ' ' << walls_it->p1_ << ' ' << walls_it->p2_ << std::endl;
    file << std::endl;
    
    // Close the file
    file.close();
}

void reset()
{
    reset_walls();
    reset_bars();
    reset_particles();
    
    selected_particle_id = -1;
}

void create_cloth(int n, double d, Vector2d bottom_left_corner, bool fix)
{
    double x0 = bottom_left_corner.x;
    double y0 = bottom_left_corner.y;
    int id0 = particles.size();
    
    bool fixed = false;
    
    // Create particles
    for (int j = 0; j < n; j++)
    {
        if (fix)
            fixed = (j == n-1) ? true : false;
        
        for (int i = 0; i < n; i++)
        {
            if (fixed)
                Particle::create(x0 + i * d, y0 + j * d, true);
            else
                Particle::create(x0 + i * d, y0 + j * d, false);
        }
    }
    
    // Create horizontal connections
    for (int j = 0; j < n; j++)
        for (int i = 0; i < n-1; i++)
            Bar::create(id0 + j * n + i, id0 + j * n + i + 1, 0.0, ROOM_TEMPERATURE);
    
    // Create vertical connections
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n-1; j++)
            Bar::create(id0 + i + n * j, id0 + i + n * (j + 1), 0.0, ROOM_TEMPERATURE);
}

std::string date_str()
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    std::ostringstream s;
    
    s << timeinfo->tm_year + 1900 << '-' << timeinfo->tm_mon + 1 << '-' << timeinfo->tm_mday;
    
    return s.str();
}

std::string time_str()
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    std::ostringstream s;

    s << timeinfo->tm_hour << '-' << timeinfo->tm_min << '-' << timeinfo->tm_sec;
    
    return s.str();
}
