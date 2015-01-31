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

#define ROOM_TEMPERATURE 288
#define ENERGY_ABSORPTION 0.5
#define GRAVITY 9.81

// * * * * * * * * * * //
extern unsigned long long int simulation_time; // In us
extern unsigned long long int t; // In us
extern unsigned long long int prev_t; // In us
extern double delta_t; // In s
extern bool gravity;
extern double environment_temp;

// * * * * * * * * * * //
void microsecond_time (unsigned long long &t);
void update_time();
void update_simulation();

#endif /* defined(__Trusses__physics__) */
