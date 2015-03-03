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
    Vector2d position; // In gl coords, pinned to the centre of the label
    Vector2d offset; // In pixels
    unsigned long long int max_time; // In microseconds
    unsigned long long int time;
    int id_;
    float alpha();
    bool centre;
    
    // Anchored to the middle
    static int create(std::string str, double pos_x, double pos_y, int off_x, int off_y, double t, bool centre_l = true);
    static int destroy(int obj_id);
    static int update(int obj_id);
private:
    TempLabel(std::string str, double pos_x, double pos_y, int off_x, int off_y, double t, bool centre_l = true)
        {text = str; position = Vector2d(pos_x, pos_y); offset = Vector2d(off_x, off_y); max_time = t; time = 0; centre = centre_l;};
};

extern SlotMap<TempLabel> temp_labels;

#endif /* defined(__Trusses__temporary_label__) */
