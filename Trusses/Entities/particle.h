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
#include "slot_map.h"
#include "fixed_size.h"

class Renderer;
class Bar;

class Particle
{
    friend class Renderer;
    friend class Bar;
    friend void print_particles();
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
    
    // For tracing the path of the particle.
    bool trace_on;
    void untrace();
    
    // Numerical simulation.
    void update();
    
    virtual void draw(const Renderer& rend) const;
    
    // Remove a particle with this id
    static int destroy(int removed_id);
    
    static int create(double a, double b, bool fixed);
    
protected:
    Particle(double a, double b): trace(500) {position_.x = a; position_.y = b;}
    
    // id's of all the bars connected to this particle
    std::vector<int> bars_connected;
    
    FixedSizeContainer<Vector2d> trace;
};

void print_particles();

extern SlotMap<Particle> particles;

#endif /* defined(__Trusses__particle__) */
