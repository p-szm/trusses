//
//  particle.h
//  Trusses
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__particle__
#define __Trusses__particle__

#include <vector>
#include "math.h"
#include "pointer_slot_map.h"
#include "fixed_size.h"

class Particle
{
public:
    // Unique id of the particle
    int id_;
    
    // Needed by the simulation
    Vector2d position_;
    Vector2d prev_position_;
    Vector2d prev_position_verlet_;
    Vector2d velocity_;
    Vector2d acceleration_;
    double mass_;
    
    // Added by dragging the particle
    // TODO: Merge it with acceleration_
    Vector2d external_acceleration_;
    
    // If true the particle doesn't move.
    bool fixed_;
    
    // id's of all the bars connected to this particle
    std::vector<int> bars_connected;
    
    // For tracing the path of the particle.
    bool trace_on;
    FixedSizeContainer<Vector2d> trace;
    
    // Numerical simulation.
    void update();
    
    // Impose constraints on other entities.
    virtual void impose_constraints() = 0;
    
    // Remove a particle with this id
    static int destroy(int removed_id);
    
    Particle(double a, double b): trace(500) {position_.x = a; position_.y = b;}
    virtual ~Particle() {};
};

void print_particles();

extern PSlotMap<Particle*> particles;

#endif /* defined(__Trusses__particle__) */
