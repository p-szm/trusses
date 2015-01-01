//
//  save.h
//  Oscillator
//
//  Created by Patrick Szmucer on 01/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Oscillator__save__
#define __Oscillator__save__

#include <iostream>

void print_particles();
void print_bars();
void print_time();

int load(std::string filename);

void reset();

#endif /* defined(__Oscillator__save__) */
