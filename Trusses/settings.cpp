//
//  settings.cpp
//  Trusses
//
//  Created by Patrick on 11/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "settings.h"
#include <stdexcept>

Settings settings;

Settings::Settings()
{
    accelerations = false;
    velocities = false;
    lengths = false;
    extensions = false;
    grid = true;
    ids = false;
    show_particles = true;
    draw_bounding_boxes = false;
    draw_triangulation = false;
    bars_color_mode = STRAIN_C;
    gravity = true;
}

bool& Settings::bool_member(bool_settings bool_var)
{
    switch (bool_var) {
        case ACCELERATIONS:
            return accelerations;
        case VELOCITIES:
            return velocities;
        case LENGTHS:
            return lengths;
        case EXTENSIONS:
            return extensions;
        case GRID:
            return grid;
        case IDS:
            return ids;
        case PARTICLES:
            return show_particles;
        case BOUNDING_BOXES:
            return draw_bounding_boxes;
        case TRIANGULATION:
            return draw_triangulation;
        case GRAVITY:
            return gravity;
        default:
            throw std::invalid_argument("Variable does not exist.");
    }
}

void Settings::set(bool_settings bool_var, bool state)
{
    bool& member = bool_member(bool_var);
    member = state;
}

void Settings::toggle(bool_settings bool_var)
{
    bool& member = bool_member(bool_var);
    member = !member;
}

bool Settings::get(bool_settings bool_var)
{
    bool& member = bool_member(bool_var);
    return member;
}