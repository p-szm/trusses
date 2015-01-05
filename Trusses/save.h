//
//  save.h
//  Trusses
//
//  Created by Patrick Szmucer on 01/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__save__
#define __Trusses__save__

#include <iostream>
#include "math.h"

int load(std::string filename);
void save(std::string filename);

void reset();

void create_cloth(int n, double d, Vector2d bottom_left_corner, bool fix);

std::string date_str();
std::string time_str();

#endif /* defined(__Trusses__save__) */
