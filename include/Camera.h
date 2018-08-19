#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3d.h"
#include "Ray.h"
#include "CameraData.h"


class Camera
{
    public:

        Camera(const Vec3d& e,
               const Vec3d& l,
               const Vec3d& u,
               double d,
               double p,
               int vres,
               int hres);

        Ray getRay(int r, int c) const;

        void setup();

        CameraData exportRays() const;

        virtual ~Camera();

        int getVerticalResolution() const{ return vres;}
        int getHorizontalResolution() const{ return hres;}

    protected:
        Vec3d u, v, w;
        Vec3d eye;
        Vec3d lkp;
        Vec3d upv;
        double dist;
        double psize;
        int hres;
        int vres;
    private:
};

#endif // CAMERA_H
