//
//  physics.h
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Oscillator__physics__
#define __Oscillator__physics__

#include <iostream>
#include <sys/time.h>
#include "particle.h"

extern unsigned long long int t;
extern unsigned long long int prev_t;
extern double delta_t;
extern bool gravity;

void microsecond_time (unsigned long long &t);
void update_time();
void update_position(std::vector<Particle>& particles_vect);
double energy(std::vector<Particle>& particles);
void check_boundaries(Particle& p);
void integrate(Particle& p, Vector2d acc, double dt);

#endif /* defined(__Oscillator__physics__) */
