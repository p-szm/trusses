//
//  bar.h
//  Trusses
//
//  Created by Patrick Szmucer on 03/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__bar__
#define __Trusses__bar__

#include "pointer_slot_map.h"

#include "math.h"

#define MAX_STRAIN 0.3
#define MELTING_POINT 1800
#define STIFFNESS_AT_TM 0.5
#define THERMAL_COEFF 2e-5

struct Bar
{
    int id_;
    int p1_id;
    int p2_id;
    double r0; // r0 is an equilibrium length at given temperature. TODO: change this to r_unstressed
    double r_0K; // r_0K is bar's length at 0K
    double length() const;
    double extension() const;
    Vector2d unit12() const;
    Vector2d unit21() const;
    double stiffness; // TODO. Can stiffness just be set to sth? Currently it's a function of temperature...
    
    void set_temperature(double t);
    double get_temperature() const;
    
    void impose_constraint();
    
    int update();
    
    // Split the bar into n_parts bars of equal lengths
    // static function, because this bar has to be removed
    // in the process.
    static void split(int bar_id, unsigned int n_parts);
    
    void set_strain(double e);
    double get_strain() const;
    
    static int create(int id1, int id2, double e, double temp);
    static int destroy(int obj_id);
private:
    double temperature; // Private because it shouldn't be set without setting r0
    Bar(int id1, int id2) {p1_id = id1; p2_id = id2;}
};

void reset_bars();
void print_bars();

extern PSlotMap<Bar*> bars;

#endif /* defined(__Trusses__bar__) */
