//
//  pin_joint.h
//  Trusses
//
//  Created by Patrick on 26/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__pin_joint__
#define __Trusses__pin_joint__

#include <stdio.h>
#include "particle.h"

class Renderer;

class PinJoint: public Particle
{
public:
    static int create(double a, double b, bool fixed);
    void impose_constraints() {}
    void draw(const Renderer& rend) const;
    ~PinJoint() {}
private:
    PinJoint(double a, double b): Particle(a, b) {}
};

#endif /* defined(__Trusses__pin_joint__) */
