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
#include "math.h"

int load(std::string filename);
void save(std::string filename);

void reset();

void create_cloth(int n, double d, Vector2d bottom_left_corner, bool fix);

#endif /* defined(__Oscillator__save__) */
