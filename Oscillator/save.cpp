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

template<typename T>
void read_numbers(std::string str, std::vector<T> & target_v)
{
    // Convert the string to the stringstream
    std::istringstream s(str.substr(str.find(' ')+1, std::string::npos));
    
    std::vector<T> numbers;
    
    // Read every "word" of a stringstream
    while (s)
    {
        T n;
        
        // If s is a number
        if (s >> n)
        {
            numbers.push_back(n);
        }
    }
    
    // Put it in the target vector
    for (unsigned i = 0; i < 3; i++)
    {
        target_v.push_back(numbers[i]);
    }
}

int load(std::string filename)
{
    // TODO
    // Check if the file is valid
    
    // Remove all the existing particles and bars
    reset();
    
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

            particles.push_back(Particle::create(v[0], v[1], false));
        }
        
        // A fixed particle
        else if (line.substr(0, 1) == "f")
        {
            std::vector<double> v;
            read_numbers(line, v);
            
            particles.push_back(Particle::create(v[0], v[1], true));
        }
        
        // A bar
        else if (line.substr(0, 1) == "b")
        {
            std::vector<int> v;
            read_numbers(line, v);
            
            bars.push_back(Bar::create(v[0], v[1]));
        }
        
        // A wall
        else if (line.substr(0, 1) == "w")
        {
            std::vector<int> v;
            read_numbers(line, v);
            
            walls.push_back(Wall::create(Vector2d(v[0], v[1]), v[2], v[3]));
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
    file << date_str() << ' ' << time_str() << std::endl << std::endl;;
    
    // Print particles
    for (int i = 0; i < particles_number; i++)
    {
        if (particles[i].fixed)
            file << 'f';
        else
            file << 'p';
        file << particles[i].id << ' ' << particles[i].position;
        file << std::endl;
    }
    file << std::endl;
    
    // Print bars
    for (int i = 0; i < bars_number; i++)
    {
        file << 'b' << bars[i].id << ' ' << bars[i].p1_id << ' ' << bars[i].p2_id << std::endl;
    }
    file << std::endl;
    
    // Print walls
    for (int i = 0; i < walls_number; i++)
    {
        file << 'w' << walls[i].id << ' ' << walls[i].centre_ << ' ' << walls[i].width_ << ' ' << walls[i].height_ << std::endl;
    }
    file << std::endl;
    
    // Close the file
    file.close();
}

void reset()
{
    particles.clear();
    bars.clear();
    walls.clear();
    
    particles_number = 0;
    bars_number = 0;
    walls_number = 0;
    
    selected_particle_id = -1;
}

void create_cloth(int n, double d, Vector2d bottom_left_corner, bool fix)
{
    double x0 = bottom_left_corner.x;
    double y0 = bottom_left_corner.y;
    int id0 = particles_number;
    
    bool fixed = false;
    
    // Create particles
    for (int j = 0; j < n; j++)
    {
        if (fix)
            fixed = (j == n-1) ? true : false;
        
        for (int i = 0; i < n; i++)
        {
            if (fixed)
                particles.push_back(Particle::create(x0 + i * d, y0 + j * d, true));
            else
                particles.push_back(Particle::create(x0 + i * d, y0 + j * d, false));
        }
    }
    
    // Create horizontal connections
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n-1; i++)
        {
            bars.push_back(Bar::create(id0 + j * n + i, id0 + j * n + i + 1));
        }
    }
    
    // Create vertical connections
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n-1; j++)
        {
            bars.push_back(Bar::create(id0 + i + n * j, id0 + i + n * (j + 1)));
        }
    }
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
