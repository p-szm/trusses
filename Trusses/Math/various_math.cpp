//
//  math.cpp
//  Trusses
//
//  Created by Patrick on 24/03/2015.
//  Copyright (c) 2015 Patrick Szmucer. All rights reserved.
//

#include <stdio.h>
#include <cstdlib>
#include "math.h"

Vector2d Vector2d::reflect(const Vector2d& normal, const Vector2d& intersection_point)
{
    return *this - 2 * ((*this - intersection_point) * normal) * normal;
}

Vector2d Vector2d::bisect(const Vector2d& v1, const Vector2d& v2)
{
    return (v1.norm()+ v2.norm()).norm();
}

bool Segment::intersect(const Segment& seg, Vector2d& point, double& t, double& u)
{
    // Check the bounding boxes of the segments first,
    // that is don't check for the intersection if the segments
    // are too far away.
    if (min(p1.x, p2.x) > max(seg.p1.x, seg.p2.x) || max(p1.x, p2.x) < min(seg.p1.x, p2.x)
        || min(p1.y, p2.y) > max(seg.p1.y, seg.p2.y) || max(p1.y, p2.y) < min(seg.p1.y, seg.p2.y))
        return false;
    
    Vector2d p = p1;
    Vector2d r = p2 - p1;
    Vector2d q = seg.p1;
    Vector2d s = seg.p2 - seg.p1;
    
    Vector2d pq = q - p;
    double rxs = r.cross(s);
    
    // rxs can be arbitrarily small, because we don't care
    // if 1/rxs blows up.
    if (abs_d(rxs) == 0)
        return false;
    
    t = pq.cross(s) / rxs;
    u = pq.cross(r) / rxs;
    //std::cout << "tu: " << t << " " << u << std::endl;
    
    if (t < 0 || t > 1 || u < 0 || u > 1)
        return false;
    
    point = p + t * r;
    return true;
}

bool Segment::intersect(const Segment &seg, Vector2d& point)
{
    double t;
    double u;
    return intersect(seg, point, t, u);
}

double Segment::dist2(Vector2d& p) const
{
    Vector2d u = (p2-p1).norm();
    return ((p-p1) - ((p-p1)*u)*u).abs2();
}

double Segment::dist(Vector2d& p) const
{
    return sqrt(dist(p));
}

double random(double range)
{
    double r = 1.0 * rand() / RAND_MAX;
    return r * 2 * range - range;
}

double abs_d(double x)
{
    if (x >= 0.0)
        return x;
    else
        return -x;
}

double round(double x)
{
    x += 0.5;
    return floor(x);
}

double min(double a, double b)
{
    if (a >= b)
        return b;
    else
        return a;
}

double max(double a, double b)
{
    if (a >= b)
        return a;
    else
        return b;
}