//
//  various_math.h
//  Trusses
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__various_math__
#define __Trusses__various_math__

#include <iostream>
#include <cmath>

#define SMALL_NUM 1e-5
#define RADPERDEG 0.01745329251

// * * * * * * * * * * //
double random(double range);
double abs_d(double x);
double round(double x);
double min(double a, double b);
double max(double a, double b);

#endif /* defined(__Trusses__various_math__) */
