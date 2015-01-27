//
//  math.h
//  Trusses
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__math__
#define __Trusses__math__

#include <iostream>
#include <cmath>

#define SMALL_NUM 1e-5

struct Vector2d
{
    double x;
    double y;
    Vector2d(float a, float b): x(a), y(b) {}
    Vector2d() {};
    Vector2d operator+ (const Vector2d &v) { return Vector2d(x+v.x, y+v.y); }
    Vector2d operator- (const Vector2d &v) { return Vector2d(x-v.x, y-v.y); }
    double operator* (const Vector2d &v) { return (x*v.x + y*v.y); }
    Vector2d operator/ (const double &a) { return Vector2d(x/a, y/a); }
    friend Vector2d operator* (const double &a, const Vector2d &v) { return Vector2d(v.x*a, v.y*a); }
    friend Vector2d operator- (const Vector2d &v) { return Vector2d(-v.x, -v.y); }
    double abs2() const { return (x*x + y*y); }
    double abs() const { return sqrt(this->abs2()); }
    Vector2d norm() const { double s(this->abs()); if (s==0) return *this; else return Vector2d(x/s, y/s); }
    Vector2d& operator+= (const Vector2d &v) { x+=v.x; y+=v.y; return *this; }
    Vector2d& operator-= (const Vector2d &v) { x-=v.x; y-=v.y; return *this; }
    
    friend std::ostream& operator << (std::ostream &out, const Vector2d &v) { out << v.x << ' ' << v.y; return out; }
    
};

float random(float range);
double abs_d(double x);
double round(double x);

double min(double a, double b);
double max(double a, double b);

#endif /* defined(__Trusses__math__) */
