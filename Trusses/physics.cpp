//
//  physics.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "physics.h"

#include "particle.h"
#include "wall.h"
#include "bar.h"

#include <sys/time.h>

unsigned long long int t = 0.0;
unsigned long long int prev_t = 0.0;
double delta_t = 0.0;
double environment_temp = ROOM_TEMPERATURE;

#define RELAX_ITER 30
#define HORIZON 1000

bool gravity = false;

void microsecond_time (unsigned long long &t)
// Returns system time in microseconds, used for introducing delays at low simulation speeds
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    t = (unsigned long long)tv.tv_usec + 1000000 * (unsigned long long)tv.tv_sec;
}

void update_time()
{
    prev_t = t;
    microsecond_time(t);
    delta_t = (t - prev_t)/1000000.0;
}

void update_simulation()
{
    SlotMap<Particle>::iterator particles_it;
    SlotMap<Bar>::iterator bars_it;
    
    // Update each particle's position by Verlet integration
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
        particles_it->update();
    
    // Use relaxation to satisfy the constraints imposed by bars
    // Large RELAX_ITER means accurate simulation, and hence stiff bars
    for (int j = 0; j < RELAX_ITER; j++)
        for (bars_it = bars.begin(); bars_it != bars.end(); bars_it++)
            bars_it->impose_constraint();
    
    // Collisions of particles with walls
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
        particles_it->impose_boundaries();
    
    // Update each bar and add see if it will be destroyed
    std::vector<int> bars_to_destroy;
    for (bars_it = bars.begin(); bars_it != bars.end(); bars_it++)
        if (bars_it->update())
            bars_to_destroy.push_back(bars_it->id_);
    
    // Destroy each bar that was previously added to the list
    for (int i = 0; i < bars_to_destroy.size(); i++)
        Bar::destroy(bars_to_destroy[i]);
    
    // Destroy particles which are very far away
    std::vector<int> particles_to_destroy;
    for (particles_it = particles.begin(); particles_it != particles.end(); particles_it++)
    {
        if (abs_d(particles_it->position_.x) > HORIZON || abs_d(particles_it->position_.y) > HORIZON)
            particles_to_destroy.push_back(particles_it->id_);
    }
    for (int i = 0; i < particles_to_destroy.size(); i++)
        Particle::destroy(particles_to_destroy[i]);
}