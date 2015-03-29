//
//  physics.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#include "physics.h"

#include "particle.h"
#include "wall.h"
#include "bar.h"
#include "obstacle.h"

#include <sys/time.h>

#define RELAX_ITER 30

// * * * * * * * * * * //
unsigned long long int simulation_time = 0;
unsigned long long int t = 0;
unsigned long long int prev_t = 0;
double delta_t = 0.0;
bool gravity = true;
double environment_temp = ROOM_TEMPERATURE;

// * * * * * * * * * * //
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
    // Update each particle's position by Verlet integration
    for (int i = 0; i < particles.size(); i++)
        particles.at(i).update();
    
    // Use relaxation to satisfy the constraints imposed by bars
    // Large RELAX_ITER means accurate simulation, and hence stiff bars
    for (int j = 0; j < RELAX_ITER; j++)
        for (int i = 0; i < bars.size(); i++)
            bars.at(i).impose_constraint();
    
    // Collisions of particles with obstacles
    for (int i = 0; i < obstacles.size(); i++)
        obstacles.at(i).collide();
    
    // Collisions of particles with walls
    for (int i = 0; i < walls.size(); i++)
        walls.at(i).collide();
    
    // Update each bar and add see if it will be destroyed
    std::vector<int> bars_to_destroy;
    for (int i = 0; i < bars.size(); i++)
    {
        Bar& b = bars.at(i);
        if (b.update())
            bars_to_destroy.push_back(b.id_);
    }
    
    // Destroy each bar that was previously added to the list
    for (int i = 0; i < bars_to_destroy.size(); i++)
        Bar::destroy(bars_to_destroy[i]);
    
    // Destroy particles which are very far away
    std::vector<int> particles_to_destroy;
    for (int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles.at(i);
        if (abs_d(p.position_.x) > HORIZON || abs_d(p.position_.y) > HORIZON)
            particles_to_destroy.push_back(p.id_);
    }
    for (int i = 0; i < particles_to_destroy.size(); i++)
        Particle::destroy(particles_to_destroy[i]);
}