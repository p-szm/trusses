//
//  particle.h
//  Oscillator
//
//  Created by Patrick Szmucer on 30/12/2014.
//  Copyright (c) 2014 Patrick Szmucer. All rights reserved.
//

#ifndef __Oscillator__particle__
#define __Oscillator__particle__

#include <stdio.h>
#include <random>

#define MAX_VELOCITY 0.1

// TODO: remove this
extern double delta_t;

float random(float range);

struct Vector2d
{
    double x;
    double y;
    Vector2d(float a, float b): x(a), y(b) {}
    Vector2d() {};
    Vector2d operator+ (const Vector2d &v) { return Vector2d(x+v.x, y+v.y); }
    Vector2d operator- (const Vector2d &v) { return Vector2d(x-v.x, y-v.y); }
    double operator* (const Vector2d &v) { return (x*v.x + y*v.y); }
    friend Vector2d operator* (const double &a, const Vector2d &v) { return Vector2d(v.x*a, v.y*a); }
    double abs2() { return (x*x + y*y); }
    double abs() { return sqrt(this->abs2()); }
    Vector2d norm() { double s(this->abs()); if (s==0) return *this; else return Vector2d(x/s, y/s); }

};

struct Particle
{
    Vector2d position;
    Vector2d prev_position;
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
private:
    Particle(double a, double b) {position.x = a; position.y = b;}
};

struct Bar
{
    int p1_id;
    int p2_id;
    double k;
    double lambda;
    Bar(int id1, int id2) {p1_id = id1; p2_id = id2;}
    double length();
};

extern std::vector<Particle> particles;
extern std::vector<Bar> bars;

extern int selected_particle_id;
extern int particles_number;

#endif /* defined(__Oscillator__particle__) */
