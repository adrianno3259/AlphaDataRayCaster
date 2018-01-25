#include "Ray.h"

Ray::Ray(const Vec3d& o, const Vec3d& d) :
    origin(o), direction(d) { }

Vec3d Ray::rayPoint(double t) const
{
    return origin + t * direction;
}
