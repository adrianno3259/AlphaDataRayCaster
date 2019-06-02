#ifndef RAY_H
#define RAY_H

#include "Vec3d.h"


/**
*   Mathematical representation of a light ray with
*   an origin point and a direction vector
*/
class Ray
{
    public:
        /// Origin point of the ray
        Vec3d origin;
        /// Direction vector of the ray
        Vec3d direction;
        /// Ray identifier number
        long long int id;

        /**
        *   Default constructor
        *   Sets a ray starting on the origin point o
        *   and following the direction of the vector d
        */
        Ray(const Vec3d& o, const Vec3d& d);

        /**
        *   Calculates the point corresponding to
        *   the parameter t in the ray
        */
        Vec3d rayPoint(double t) const;

        static const int NUM_ATTRIBUTES = 6;
};

#endif // RAY_H
