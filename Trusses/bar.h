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
#include "slot_map.h"

#include "math.h"

struct Bar
{
    int id_;
    int p1_id;
    int p2_id;
    double r0;
    double length() const;
    double extension() const;
    Vector2d unit12() const;
    Vector2d unit21() const;
    
    static int create(int id1, int id2);
    static int destroy(int obj_id);
private:
    Bar(int id1, int id2) {p1_id = id1; p2_id = id2;}
};

void reset_bars();
void print_bars();

extern SlotMap<Bar> bars;

#endif /* defined(__Trusses__bar__) */
