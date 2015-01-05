//
//  particle.h
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__particle__
#define __Trusses__particle__

#include <random>

#include "math.h"

// TODO: remove this


struct Particle
{
    Vector2d position_;
    Vector2d prev_position_;
    Vector2d prev_position_verlet_;
    Vector2d velocity_;
    Vector2d acceleration_;
    Vector2d external_acceleration_; // Added by dragging the particle
    double mass_;
    
    bool fixed_;
    bool dragged_;
    
    int id_;
    
    static Particle create(double a, double b, bool fixed);
private:
    Particle(double a, double b) {position_.x = a; position_.y = b;}
};

extern std::vector<Particle> particles;
extern int particles_number;

#endif /* defined(__Trusses__particle__) */
