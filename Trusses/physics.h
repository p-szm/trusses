//
//  physics.h
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__physics__
#define __Trusses__physics__

#include "particle.h"

extern unsigned long long int t;
extern unsigned long long int prev_t;
extern double delta_t;
extern bool gravity;

void microsecond_time (unsigned long long &t);
void update_time();
void update_position();
void check_boundaries(Particle& p);
void integrate(Particle& p, Vector2d acc, double dt);

#endif /* defined(__Trusses__physics__) */
