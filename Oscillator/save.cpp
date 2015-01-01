//
//  save.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 01/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "save.h"
#include "particle.h"
#include <time.h>
#include <fstream>
#include <sstream>
#include <vector>

void print_particles()
{
    if (particles_number == 0)
    {
        std::cout << "Nothing to save" << std::endl;
        return;
    }
    
    for (int i = 0; i < particles_number; i++)
    {
        std::cout << 'p' << particles[i].id << ' ' << particles[i].position;
        if (particles[i].fixed == true)
            std::cout << " f";
        std::cout << std::endl;
    }
}

void print_bars()
{
    for (int i = 0; i < bars_number; i++)
    {
        std::cout << 'b' << bars[i].id << ' ' << bars[i].p1_id << ' ' << bars[i].p2_id << std::endl;
    }
}

void print_time()
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    //std::cout << asctime (timeinfo) << std::endl;
    std::cout << timeinfo->tm_year + 1900 << '-' << timeinfo->tm_mon + 1 << '-' << timeinfo->tm_mday << ' ';
    std::cout << timeinfo->tm_hour << ':' << timeinfo->tm_min << ':' << timeinfo->tm_sec << std::endl;
}

// struct tm {
// int	tm_sec;		/* seconds after the minute [0-60] */
// int	tm_min;		/* minutes after the hour [0-59] */
// int	tm_hour;	/* hours since midnight [0-23] */
// int	tm_mday;	/* day of the month [1-31] */
// int	tm_mon;		/* months since January [0-11] */
// int	tm_year;	/* years since 1900 */
// int	tm_wday;	/* days since Sunday [0-6] */
// int	tm_yday;	/* days since January 1 [0-365] */
// int	tm_isdst;	/* Daylight Savings Time flag */
// long	tm_gmtoff;	/* offset from CUT in seconds */
// char	*tm_zone;	/* timezone abbreviation */
// };

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
            std::cout << n << std::endl;
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
    // Load fixed particles
    
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
        
        // A bar
        else if (line.substr(0, 1) == "b")
        {
            std::vector<int> v;
            read_numbers(line, v);
            
            bars.push_back(Bar::create(v[0], v[1]));
        }
    }
    
    // Close the file
    file.close();
    
    return 0;
}

void reset()
{
    particles.clear();
    bars.clear();
    particles_number = 0;
    bars_number = 0;
    selected_particle_id = -1;
}
