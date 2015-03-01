//
//  temporary_label.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 31/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "temporary_label.h"
#include "physics.h"

SlotMap<TempLabel> temp_labels;

int TempLabel::create(std::string str, double pos_x, double pos_y, int off_x, int off_y, double t)
{
    TempLabel new_label(str, pos_x, pos_y, off_x, off_y, t);
    int new_id = temp_labels.add(new_label);
    return new_id;
}

int TempLabel::destroy(int obj_id)
{
    int result = temp_labels.remove(obj_id);
    return result;
}

// The return value of 1 means "remove me"
int TempLabel::update(int obj_id)
{
    if (temp_labels.exists(obj_id))
    {
        TempLabel* label = &temp_labels[obj_id];
        label->time += (t - prev_t);
        if (label->time > label->max_time)
            return 1;
    }
    return 0;
}

float TempLabel::alpha()
{
    float time_fraction = 1.0 * time / max_time;
    if (time_fraction > 0.7)
        return (1 - time_fraction) / 0.3;
    return 1.0;
}