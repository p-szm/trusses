//
//  grid.h
//  Trusses
//
//  Created by Patrick on 11/04/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__grid__
#define __Trusses__grid__

struct Grid
{
    friend class Renderer;
    friend class Mouse;
public:
    Grid(int dist_px);
private:
    // In px
    int grid_dist_px;
};

extern Grid grid;

#endif /* defined(__Trusses__grid__) */
