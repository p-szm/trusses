//
//  temporary_label.h
//  Trusses
//
//  Created by Patrick Szmucer on 31/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__temporary_label__
#define __Trusses__temporary_label__

#include <iostream>
#include "math.h"
#include "slot_map.h"

class TempLabel
{
public:
    std::string text;
    Vector2d position;
    unsigned long long int max_time; // In microseconds
    unsigned long long int time;
    int id_;
    float alpha();
    
    static int create(std::string str, Vector2d pos, double t);
    static int destroy(int obj_id);
    static int update(int obj_id);
    
private:
    TempLabel(std::string str, Vector2d pos, double t):
        text(str), position(pos), max_time(t) {time = 0;};
    TempLabel(std::string str, float x, float y, double t):
        text(str), position(Vector2d(x, y)), max_time(t) {time = 0;};
};

extern SlotMap<TempLabel> temp_labels;

#endif /* defined(__Trusses__temporary_label__) */
