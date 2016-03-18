//
//  segment.cpp
//  Trusses
//
//  Created by Patrick on 20/06/2015.
//
//

#include "segment.h"
#include "various_math.h"

bool Segment::intersect(const Segment& seg, Vector2d& point, double& t, double& u)
{
    // Check the bounding boxes of the segments first,
    // that is don't check for the intersection if the segments
    // are too far away.
    if (min(p1.x, p2.x) > max(seg.p1.x, seg.p2.x) || 
        max(p1.x, p2.x) < min(seg.p1.x, seg.p2.x) || 
        min(p1.y, p2.y) > max(seg.p1.y, seg.p2.y) || 
        max(p1.y, p2.y) < min(seg.p1.y, seg.p2.y))
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