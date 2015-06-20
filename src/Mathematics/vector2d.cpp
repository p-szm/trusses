//
//  vector2d.cpp
//  Trusses
//
//  Created by Patrick on 20/06/2015.
//
//

#include "vector2d.h"

Vector2d Vector2d::reflect(const Vector2d& normal, const Vector2d& intersection_point)
{
    return *this - 2 * ((*this - intersection_point) * normal) * normal;
}

Vector2d Vector2d::bisect(const Vector2d& v1, const Vector2d& v2)
{
    return (v1.norm()+ v2.norm()).norm();
}