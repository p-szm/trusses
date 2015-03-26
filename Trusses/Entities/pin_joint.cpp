//
//  pin_joint.cpp
//  Trusses
//
//  Created by Patrick on 26/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include "pin_joint.h"
#include "physics.h"

int PinJoint::create(double a, double b, bool fixed)
{
    PinJoint* new_pjoint = new PinJoint(a, b);
    
    new_pjoint->velocity_ = Vector2d(0.0, 0.0);
    new_pjoint->prev_position_ = new_pjoint->position_ - delta_t * new_pjoint->velocity_;
    new_pjoint->prev_position_verlet_ = new_pjoint->position_ - delta_t * new_pjoint->velocity_;
    new_pjoint->acceleration_ = Vector2d(0.0, 0.0);
    new_pjoint->external_acceleration_ = Vector2d(0.0, 0.0);
    new_pjoint->mass_ = 1.0;
    
    new_pjoint->fixed_ = (fixed) ? true : false;
    
    // Trace
    new_pjoint->trace_on = false;
    
    int new_id = particles.add(new_pjoint);
    return new_id;
}