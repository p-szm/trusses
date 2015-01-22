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

enum OSCIL_T {X, Y, CW, ACW};

struct Oscillation
{
    Vector2d origin;
    double amplitude;
    double frequency;
    double phase;
    Oscillation(Vector2d orig, double amp, double freq): origin(orig), amplitude(amp), frequency(freq) {}
    Oscillation() {}
};

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
    
    std::vector<int> bars_connected;
    
    // For oscillations of fixed particles
    Oscillation oscillation_;
    OSCIL_T oscil_dir;
    // Moves the particle using its motion function
    void move();
    
    static int create(double a, double b, bool fixed);
    static void destroy(int removed_id);
    
private:
    Particle(double a, double b) {position_.x = a; position_.y = b;}
};

int particle_location(int id);

void print_particles();

void reset_particles();

extern std::vector<Particle> particles;
extern int particles_number;

#endif /* defined(__Trusses__particle__) */
