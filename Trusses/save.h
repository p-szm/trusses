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

#define SAVE_PATH "/Users/patrick/workspace/saves/"

class Vector2d;

// * * * * * * * * * * //
int load(std::string filename);
void save(std::string filename);
void reset_everything();
void create_cloth(int n, double d, Vector2d bottom_left_corner, bool fix);

// * * * * * * * * * * //
std::string date_str();
std::string time_str();

#endif /* defined(__Trusses__save__) */
