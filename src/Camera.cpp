#include "Camera.h"
#include <cstdlib>
#include <iostream>



Camera::Camera(const Vec3d& e,
               const Vec3d& l,
               const Vec3d& up,
               double d,
               double p,
               int vres,
               int hres) :
        eye(e), lkp(l), upv(up), dist(d),
        vres(vres), hres(hres), psize(p)
{
    //printVec(e);
    w = (e - l);
    w.normalize();
    u = upv ^ w;
    u.normalize();
    v = w ^ u;


}


Ray Camera::getRay(int r, int c) const
{
    double xv = psize*(c - hres/2),
           yv = psize*(r - vres/2);

    Vec3d d = xv*u + yv*v - dist*w;
    //printVec(d);
    d.normalize();
    //printVec(d);
    Ray ray = Ray(eye, d);
    return ray;
}

CameraData Camera::exportRays() const
{
    int nRays = vres*hres;
    double* rayData = (double*) malloc(nRays*Ray::NUM_ATTRIBUTES*sizeof(double));

        printVec(u);
    printVec(v);
    printVec(w);

    for(int r = 0; r < vres; r++)
    {
        for(int c = 0; c < hres; c++)
        {
                Ray ray = getRay(r, c);
                //std::cerr<<r<<" "<< c<<" "<< (r*hres + c)*Ray::NUM_ATTRIBUTES<<std::endl;
                rayData[(r*hres + c)*Ray::NUM_ATTRIBUTES]   = ray.origin.x;
                rayData[(r*hres + c)*Ray::NUM_ATTRIBUTES+1] = ray.origin.y;
                rayData[(r*hres + c)*Ray::NUM_ATTRIBUTES+2] = ray.origin.z;
                rayData[(r*hres + c)*Ray::NUM_ATTRIBUTES+3] = ray.direction.x;
                rayData[(r*hres + c)*Ray::NUM_ATTRIBUTES+4] = ray.direction.y;
                rayData[(r*hres + c)*Ray::NUM_ATTRIBUTES+5] = ray.direction.z;
        }
    }
    CameraData cd = CameraData(rayData, nRays);
    return cd;
}


Camera::~Camera()
{
    //dtor
}
