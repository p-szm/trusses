//
//  segment.h
//  Trusses
//
//  Created by Patrick on 20/06/2015.
//
//

#ifndef __Trusses__segment__
#define __Trusses__segment__

#include "vector2d.h"

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

#endif /* defined(__Trusses__segment__) */
