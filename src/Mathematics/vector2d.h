//
//  vector2d.h
//  Trusses
//
//  Created by Patrick on 20/06/2015.
//
//

#ifndef __Trusses__vector2d__
#define __Trusses__vector2d__

#include <cmath>
#include <iostream>

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

#endif /* defined(__Trusses__vector2d__) */
