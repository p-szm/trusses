//
//  temporary_label.cpp
//  Trusses
//
//  Created by Patrick Szmucer on 31/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "temporary_label.h"
#include "physics.h"
#include "graphics.h"
#include "renderer.h"

SlotMap<TempLabel> temp_labels;

TempLabel::TempLabel(std::string str, double pos_x, double pos_y, int off_x,
          int off_y, double t, bool centre_l)
{
    text = str;
    position = Vector2d(pos_x, pos_y);
    offset = Vector2d(off_x, off_y);
    max_time = t;
    centre = centre_l;
}

int TempLabel::create(std::string str, double pos_x, double pos_y, int off_x, int off_y, double t, bool centre_l)
{
    TempLabel new_label(str, pos_x, pos_y, off_x, off_y, t, centre_l);
    int new_id = temp_labels.add(new_label);
    return new_id;
}

// The return value of 1 means "remove me"
int TempLabel::update()
{
    time += (t - prev_t);
    if (time > max_time)
        return 1;
    return 0;
}

float TempLabel::alpha() const
{
    float time_fraction = 1.0 * time / max_time;
    if (time_fraction > 0.7)
        return (1 - time_fraction) / 0.3;
    return 1.0;
}

void TempLabel::draw(const Renderer& rend) const
{
    rend.render(*this);
}

int TempLabel::destroy(int obj_id)
{
    int result = temp_labels.remove(obj_id);
    return result;
}

void update_labels()
{
    // Update labels
    std::vector<int> labels_to_remove;
    for (int i = 0; i < temp_labels.size(); i++)
    {
        TempLabel& tl = temp_labels.at(i);
        if (tl.update())
            labels_to_remove.push_back(tl.id_);
    }
    
    // Remove labels that expired
    for (int i = 0; i < labels_to_remove.size(); i++)
        TempLabel::destroy(labels_to_remove[i]);
}

void issue_label(std::string text, unsigned long long int text_time)
{
    TempLabel::create(text, -1.0, -1.0, 20, BOTTOM_MARGIN, text_time, false);
    #ifdef LOG
    std::cout << text << std::endl;
    #endif
}