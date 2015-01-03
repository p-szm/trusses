//
//  bar.h
//  Oscillator
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Oscillator__bar__
#define __Oscillator__bar__

#include <stdio.h>
#include <vector>

#include "math.h"

struct Bar
{
    int p1_id;
    int p2_id;
    double length() const;
    double extension_rate() const;
    Vector2d unit12() const;
    Vector2d unit21() const;
    
    static Bar create(int id1, int id2);
    
    int id;
    double r0;
    double tension() const;
private:
    Bar(int id1, int id2) {p1_id = id1; p2_id = id2;}
};

extern std::vector<Bar> bars;
extern int bars_number;

#endif /* defined(__Oscillator__bar__) */
