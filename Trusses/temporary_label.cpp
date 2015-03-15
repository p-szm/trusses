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

int TempLabel::create(std::string str, double pos_x, double pos_y, int off_x, int off_y, double t, bool centre_l)
{
    TempLabel new_label(str, pos_x, pos_y, off_x, off_y, t, centre_l);
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

float TempLabel::alpha() const
{
    float time_fraction = 1.0 * time / max_time;
    if (time_fraction > 0.7)
        return (1 - time_fraction) / 0.3;
    return 1.0;
}

void update_labels()
{
    // Update labels
    std::vector<int> labels_to_remove;
    SlotMap<TempLabel>::iterator labels_it;
    for (labels_it = temp_labels.begin(); labels_it != temp_labels.end(); labels_it++)
        if (TempLabel::update(labels_it->id_))
            labels_to_remove.push_back(labels_it->id_);
    
    // Remove labels that expired
    for (int i = 0; i < labels_to_remove.size(); i++)
        TempLabel::destroy(labels_to_remove[i]);
}