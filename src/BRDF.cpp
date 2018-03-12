#include "BRDF.h"
#include "Intersection.h"
#include "../Globals.h"

BRDF::BRDF(float diffuseCoef, const Color& color):
    mDiffuseCoef(diffuseCoef), mColor(color)
{
    //ctor
}

Color BRDF::f(const Vec3d& wo,
              const Vec3d& wi,
              const Intersect& it) const
{
    return mColor * mDiffuseCoef * INV_PI;
}

BRDF::~BRDF()
{
    //dtor
}
