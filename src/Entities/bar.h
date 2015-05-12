//
//  bar.h
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__bar__
#define __Trusses__bar__

#include "slot_map.h"

#define MAX_STRAIN 0.3

class Renderer;
class Vector2d;

class Bar
{
public:
    int id_;
    
    // Ids of the particles connected to the bar
    int p1_id, p2_id;
    
    // Unit vector going from p1 to p2
    Vector2d unit12() const;
    
    // Unit vector going from p2 to p1
    Vector2d unit21() const;
    
    // Current length of the bar
    double length() const;
    
    // Absolute extension of the bar
    double extension() const;
    
    // Tension is +ve, compression is -ve
    void set_strain(double e);
    double get_strain() const;
    
    // If true, bar can be destroyed
    bool is_fractured() const;
    
    // Imposes constraints on the particles
    // it's connected to
    void impose_constraint();
    
    static int create(int id1, int id2);
    static int create(int id1, int id2, double e);
    static int destroy(int obj_id);
    
    // Split the bar into n_parts bars of equal lengths.
    // The bar is removed in the process and completely
    // new bars are created.
    static void split(int bar_id, unsigned int n_parts);
    
    void draw(const Renderer& rend) const;
    
private:
    // Equilibrium, unstressed length
    double r0;
    
    // Between 0.0 and 1.0
    double stiffness;
    
    Bar(int id1, int id2, double e);
};

void print_bars();

extern SlotMap<Bar> bars;

#endif /* defined(__Trusses__bar__) */
