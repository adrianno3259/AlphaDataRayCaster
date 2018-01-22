#include "../include/Vec3d.h"
#include <cmath>

Vec3d& Vec3d::operator= (const Vec3d& rhs){
    if (this == &rhs)
        return (*this);
    x = rhs.x; y = rhs.y; z = rhs.z;
    return (*this);
}

float Vec3d::length(void) { return sqrt((x*x) + (y*y) + (z*z)); }

float Vec3d::len_squared(void) { return (x*x) + (y*y) + (z*z); }

void Vec3d::normalize(void){
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}

Vec3d& Vec3d::hat(void){
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
	return (*this);
}
