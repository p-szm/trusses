//
//  math.cpp
//  Oscillator
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "math.h"

float random(float range)
{
    float r = 1.0 * rand() / RAND_MAX;
    return r * 2 * range - range;
}

double abs_d(double x)
{
    if (x >= 0.0)
        return x;
    else
        return -x;
}

double round(double x)
{
    x += 0.5;
    return floor(x);
}