#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <Vec3d.h>


/** this class stores an ray-object intersection point
*/
struct Intersection{

    /**
    */
    double t;

    /** The point where the intersection occurred
    */
    Vec3d hitPoint;

    /** Normal vector of the hit surface
    */
    Vec3d normalVector;

    /**
    */
    Vec3d rayDirection;

    /**
    */
    bool hit;

    /**
    */
    int triangleId;
};



#endif // INTERSECTION_H
