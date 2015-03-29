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

class Particle;
class Bar;
class Wall;
class Obstacle;
class TempLabel;
class Button;

class Renderer
{
public:
    void render(const Particle& obj) const;
    void render(const Bar& obj) const;
    void render(const Wall& obj) const;
    void render(const Obstacle& obj) const;
    void render(const TempLabel& obj) const;
    void render(const Button& obj) const;
};

#endif /* defined(__Trusses__renderer__) */
