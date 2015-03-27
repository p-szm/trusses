//
//  renderer.h
//  Trusses
//
//  Created by Patrick on 27/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__renderer__
#define __Trusses__renderer__

#include <stdio.h>

class PinJoint;
class Bar;
class Wall;
class Obstacle;
class TempLabel;
class Button;

class Renderer
{
public:
    void render(const PinJoint* const obj) const;
    void render(const Bar* const obj) const;
    void render(const Wall* const obj) const;
    void render(const Obstacle* const obj) const;
    void render(const TempLabel* const obj) const;
    void render(const Button* const obj) const;
};

#endif /* defined(__Trusses__renderer__) */
