//
//  various_math.h
//  Trusses
//
//  Created by Patrick Szmucer on 02/01/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#ifndef __Trusses__various_math__
#define __Trusses__various_math__

#include <iostream>
#include <cmath>

#define SMALL_NUM 1e-5
#define RADPERDEG 0.01745329251

// * * * * * * * * * * //
class Vector2d
{
public:
    double x;
    double y;
    Vector2d() {x = 0.0, y = 0.0;};
    Vector2d(double a, double b): x(a), y(b) {}
    Vector2d operator+ (const Vector2d &v) const { return Vector2d(x+v.x, y+v.y); }
    Vector2d operator- (const Vector2d &v) const { return Vector2d(x-v.x, y-v.y); }
    friend Vector2d operator- (const Vector2d &v) { return Vector2d(-v.x, -v.y); }
    Vector2d& operator+= (const Vector2d &v) { x+=v.x; y+=v.y; return *this; }
    Vector2d& operator-= (const Vector2d &v) { x-=v.x; y-=v.y; return *this; }
    double operator* (const Vector2d &v) const { return (x*v.x + y*v.y); }
    friend Vector2d operator* (const double &a, const Vector2d &v) { return Vector2d(v.x*a, v.y*a); }
    Vector2d operator/ (const double &a) const { return Vector2d(x/a, y/a); }
    friend Vector2d operator/ (const double &a, const Vector2d &v) { return Vector2d(v.x/a, v.y/a); }
    double cross(const Vector2d& v) { return x * v.y - y * v.x; }
    double abs2() const { return (x*x + y*y); }
    double abs() const { return std::sqrt(this->abs2()); }
    Vector2d norm() const { double s(this->abs()); if (s==0) return *this; else return Vector2d(x/s, y/s); }
    friend std::ostream& operator << (std::ostream &out, const Vector2d &v) { out << v.x << ' ' << v.y; return out; }
    Vector2d reflect(const Vector2d& normal, const Vector2d& intersection_point);
    static Vector2d bisect(const Vector2d& v1, const Vector2d& v2);
};

class Segment
{
public:
    Segment() {}
    Segment(Vector2d a, Vector2d b): p1(a), p2(b) {}
    Vector2d p1;
    Vector2d p2;
    
    // Returns true if the segments intersect and false otherwise
    bool intersect(const Segment& seg, Vector2d& point);
    bool intersect(const Segment& seg, Vector2d& point, double& t, double& u);
    
    // The distance between the point and a line defined by the segment.
    double dist2(Vector2d& p) const;
    double dist(Vector2d& p) const;
};

struct vec3
{
    double x;
    double y;
    double z;
    vec3(double a, double b, double c): x(a), y(b), z(c) {}
    vec3(): x(0), y(0), z(0) {}
};

// * * * * * * * * * * //
double random(double range);
double abs_d(double x);
double round(double x);
double min(double a, double b);
double max(double a, double b);

#endif /* defined(__Trusses__various_math__) */
