//
//  save.cpp
//  Trusses
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
#include <map>

#include "pin_joint.h"
#include "bar.h"
#include "wall.h"
#include "obstacle.h"
#include "physics.h"
#include "graphics.h"
#include "interface.h"
#include "interpreter.h"
#include "temporary_label.h"
#include "bars_tool.h"

// * * * * * * * * * * //
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

template <class T>
T number(std::string str)
{
    // Convert the string to the stringstream
    std::istringstream s(str);
    
    T n;
    if (s >> n)
        return n;
    return 0;
}

// * * * * * * * * * * //
int load(std::string filename)
{
    // TODO
    // Check if the file is valid
    
    // Important:
    // App freezes when loading walls
    // --- Fixed
    // But check the function read_numbers
    
    reset_everything();
    
    // Open the file
    std::ifstream file(filename.c_str());
    
    // Check if the file was opened successfully
    if (!file.is_open())
        return 1;
    
    // This map is necessary as particles are saved by ids,
    // and ids do not necessarily range uniformly from 0 to n-1.
    // They might have gaps, for example 0,1,2,3,5,6,8,9.
    // We therefore need to map these imported ids to 0,1,2,3,4,5,6,7
    // or etc.
    std::map<int, int> particles_map;
    
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
            {
                // TODO: Some more error checking here
                // Map the imported ids to the ids from 0...n-1
                int p_id = number<int>(line.substr(1, line.find(" ")));
                particles_map[p_id] = (int)particles_map.size();
                
                PinJoint::create(v[0], v[1], false);
            }
        }
        
        // A fixed particle
        else if (line.substr(0, 1) == "f")
        {
            std::vector<double> v;
            read_numbers(line, v);
            
            if (v.size() == 2)
            {
                // Map the imported ids to the ids from 0...n-1
                int p_id = number<int>(line.substr(1, line.find(" ")));
                particles_map[p_id] = (int)particles_map.size();
             
                PinJoint::create(v[0], v[1], true);
            }
        }
        
        // A bar
        else if (line.substr(0, 1) == "b")
        {
            std::vector<double> v;
            read_numbers(line, v);
            
            if (v.size() == 4)
                Bar::create(particles_map[v[0]], particles_map[v[1]], v[2], v[3]);
            else if (v.size() == 2)
                Bar::create(particles_map[v[0]], particles_map[v[1]], 0.0, ROOM_TEMPERATURE);
        }
        
        // A wall
        else if (line.substr(0, 1) == "w")
        {
            std::vector<double> v;
            read_numbers(line, v);
            
            if (v.size() == 4)
                Wall::create(Vector2d(v[0], v[1]), Vector2d(v[2], v[3]));
        }
        
        // An obstacle
        else if (line.substr(0, 1) == "o")
        {
            std::vector<double> v;
            read_numbers(line, v);
            
            Polygon poly;
            if (v.size() % 2 == 0)
            {
                for (size_t i = 0; i < v.size(); i+=2)
                    poly.add_point(Vector2d(v[i], v[i+1]));
                obstacles.add(new Obstacle(poly));
            }
        }
    }
    
    // Close the file
    file.close();
    
    issue_label("File loaded", INFO_LABEL_TIME);
    
    return 0;
}

void save(std::string filename)
{
    // Open the file
    std::ofstream file(filename.c_str());
    
    // Print time
    file << date_str() << ' ' << time_str() << std::endl << std::endl;
    
    // Print particles
    for (int i = 0; i < particles.size(); i++)
    {
        Particle* p = particles.at(i);
        if (p->fixed_)
            file << 'f';
        else
            file << 'p';
        file << p->id_ << ' ' << p->position_;
        file << std::endl;
    }
    file << std::endl;
    
    // Print bars
    // b-bar_id particle1_id particle2_id strain temperature
    for (int i = 0; i < bars.size(); i++)
    {
        Bar * b = bars.at(i);
        file << 'b' << b->id_ << ' ' << b->p1_id << ' ' << b->p2_id << ' ' << b->get_strain() << ' ' << b->get_temperature() << std::endl;
    }
    file << std::endl;
    
    // Print walls
    for (int i = 0; i < walls.size(); i++)
    {
        Wall* w = walls.at(i);
        file << 'w' << w->id_ << ' ' << w->p1_ << ' ' << w->p2_ << std::endl;
    }
    file << std::endl;
    
    // Print the obstacles
    for (int i = 0; i < obstacles.size(); i++)
    {
        Obstacle* ob = obstacles.at(i);
        file << "o" << ob->id_;
        for (size_t i = 0; i < ob->points.size(); i++)
            file << " " << ob->points[i];
        file << std::endl << std::endl;
    }
    
    // Close the file
    file.close();
    
    std::string text = "Saved as " + filename;
    issue_label(text, INFO_LABEL_TIME);
}

void reset_everything()
{
    reset_walls();
    reset_bars();
    reset_particles();
    reset_obstacles();
    pause_simulation();
    simulation_time = 0;
    Tool::set(current_tool, new BarsTool);
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
                PinJoint::create(x0 + i * d, y0 + j * d, true);
            else
                PinJoint::create(x0 + i * d, y0 + j * d, false);
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
