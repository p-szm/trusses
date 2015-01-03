//
//  particle.h
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Oscillator__particle__
#define __Oscillator__particle__

#include <random>

#include "math.h"

#define MAX_VELOCITY 0.1

// TODO: remove this
extern double delta_t;

struct Particle
{
    Vector2d position;
    Vector2d prev_position;
    Vector2d prev_position_verlet;
    Vector2d velocity;
    Vector2d acceleration;
    bool fixed;
    double mass;
    double r01;
    double r02;
    static Particle create(double a, double b, bool fixed);
    bool highlight;
    void fix();
    void unfix();
    int id;
    bool dragged;
    Vector2d external_acceleration;
private:
    Particle(double a, double b) {position.x = a; position.y = b;}
};

struct Bar
{
    int p1_id;
    int p2_id;
    double length() const;
    double extension_rate() const;
    Vector2d unit12() const;
    Vector2d unit21() const;
    
    static Bar create(int id1, int id2);
    
    int id;
    double r0;
    double tension() const;
private:
    Bar(int id1, int id2) {p1_id = id1; p2_id = id2;}
};

struct Wall
{
    Wall(Vector2d p1, Vector2d p2) {p1_ = p1; p2_ = p2;}
    static Wall create(Vector2d p1, Vector2d p2);
    Vector2d p1_;
    Vector2d p2_;
    int id;
    
    double x_min() const;
    double x_max() const;
    double y_min() const;
    double y_max() const;
};

extern std::vector<Particle> particles;
extern std::vector<Bar> bars;
extern std::vector<Wall> walls;

extern int particles_number;
extern int bars_number;
extern int walls_number;

#endif /* defined(__Oscillator__particle__) */
