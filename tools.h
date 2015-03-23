//
//  tools.h
//  Trusses
//
//  Created by Patrick on 22/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__tools__
#define __Trusses__tools__

#include <iostream>
#include <vector>
#include <map>
#include "math.h"

// In world coordinates
class Tool
{
public:
    virtual void mouse_click(int button, int state) = 0;
    virtual void passive() = 0;
    virtual void drag() = 0;
    virtual void display() = 0;
    virtual void key_down(unsigned char key) = 0;
    
    static void set(Tool* new_tool);
    virtual ~Tool();
};

// For drawing particles and bars
class BarsTool: public Tool
{
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display();
    void key_down(unsigned char key);
    BarsTool();
    ~BarsTool();
private:
    std::vector<int> selected_particles_ids;
};

// For drawing walls
class WallTool: public Tool
{
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display();
    void key_down(unsigned char key);
    WallTool();
    ~WallTool();
private:
    std::vector<Vector2d> points;
};

// For selecting particles and deleting them
class SelectionTool: public Tool
{
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display();
    void key_down(unsigned char key);
    SelectionTool();
    ~SelectionTool();
private:
    class Polygon
    {
    public:
        std::vector<Vector2d> points;
        void add_point(const Vector2d& p);
        size_t no_sides() const;
        bool point_inside(const Vector2d& p) const;
    };
    
    Polygon poly;
    std::map<int, bool> selection_map;
};

// For dragging particles in the simulation mode
class DragTool: public Tool
{
public:
    void mouse_click(int button, int state);
    void passive();
    void drag();
    void display();
    void key_down(unsigned char key);
    DragTool();
    ~DragTool();
private:
    int dragged_particle = -1;
    double dragging_force = 1000.0; // Newtons/px
};

#endif /* defined(__Trusses__tools__) */
