//
//  bar.h
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__bar__
#define __Trusses__bar__

#include <stdio.h>
#include <vector>

#include "math.h"

struct Bar
{
    int p1_id;
    int p2_id;
    double length() const;
    double extension() const;
    Vector2d unit12() const;
    Vector2d unit21() const;
    
    int id_;
    
    double r0;
    
    static int create(int id1, int id2);
    static void destroy(int bar_id);
    
private:
    Bar(int id1, int id2) {p1_id = id1; p2_id = id2;}
};

void reset_bars();
void print_bars();

int bar_location(int id);

extern std::vector<Bar> bars;
extern int bars_number;

#endif /* defined(__Trusses__bar__) */