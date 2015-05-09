//
//  grid.cpp
//  Trusses
//
//  Created by Patrick on 11/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "grid.h"
#include "interface.h"
#include <sstream>

Grid grid(30);

Grid::Grid(int spacing_px)
{
    spacing = spacing_px;
}

std::string Grid::to_si(double len) const
{
    std::stringstream ss;
    
    if (len < 1e-3)
        ss << len * 1e6 << "um";
    else if (len < 1e-2)
        ss << len * 1e3 << "mm";
    else if (len < 1.0)
        ss << len * 1e2 << "cm";
    else
        ss << len << "m";
    return ss.str();
}

double Grid::one_square_m() const
{
    return px_to_m(spacing);
}