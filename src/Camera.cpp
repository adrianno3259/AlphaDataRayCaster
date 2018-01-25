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
    w = (e - l);
    w.normalize();
    u = upv ^ w;
    u.normalize();
    v = w ^ u;

    printVec(u);
    printVec(v);
    printVec(w);
}


Ray Camera::getRay(int r, int c) const
{
    double xv = psize*(c - hres/2),
           yv = psize*(r - vres/2);

    Vec3d d = xv*u + yv*w - dist*w;
    d.normalize();
    Ray ray = Ray(eye, d);
    return ray;
}

CameraData Camera::exportRays() const
{
    int nRays = vres*hres;
    float* rayData = (float*) malloc(nRays*Ray::NUM_ATTRIBUTES*sizeof(float));
    for(int r = 0; r < vres; r++)
    {
        for(int c = 0; c < hres; c++)
        {
                double xv = psize*(c - hres/2),
                yv = psize*(r - vres/2);
                Vec3d d = xv*u + yv*w - d*w;
                d.normalize();

                rayData[(r*hres + c)]   = eye.x;
                rayData[(r*hres + c)+1] = eye.y;
                rayData[(r*hres + c)+2] = eye.z;
                rayData[(r*hres + c)+3] = d.x;
                rayData[(r*hres + c)+4] = d.y;
                rayData[(r*hres + c)+5] = d.z;
        }
    }
    CameraData cd = CameraData(rayData, nRays);
    return cd;
}


Camera::~Camera()
{
    //dtor
}
