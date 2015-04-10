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
class BarsTool;
class DragTool;
class ObstacleTool;
class SelectionTool;
class WallTool;
class Grid;

class Renderer
{
public:
    void render(const Particle& obj) const;
    void render(const Bar& obj) const;
    void render(const Wall& obj) const;
    void render(const Obstacle& obj) const;
    void render(const TempLabel& obj) const;
    void render(const Button& obj) const;
    void render(const BarsTool& obj) const;
    void render(const DragTool& obj) const;
    void render(const ObstacleTool& obj) const;
    void render(const SelectionTool& obj) const;
    void render(const WallTool& obj) const;
    void render(const Grid& obj) const;
};

#endif /* defined(__Trusses__renderer__) */
