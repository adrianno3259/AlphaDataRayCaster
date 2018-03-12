#include "Material.h"
#include "Intersection.h"
#include "../Globals.h"
#include <cassert>

Material::Material(float diffuseCoef, const Color& color) :
{
    this->lambertian = BRDF(diffuseCoef, color);
}

Material::~Material()
{
    //dtor
}

Color Material::shade(Intersection& it,
                      const std::vector<std::shared_ptr<Light> >& lights) const
{
    Color endColor = Color();
    Vec3d wo = -ray.direction;
    int numberOfLights = lights.size();


    for(int l = 0; l < numberOfLights; l++)
    {
        Vec3d wi = lights[l]->getDirection(ray.rayPoint(t));
        double dot = tri->normal * wi;

        if(dot > 0.0)
        {
            endColor += lights[l]->getL() * dot * lambertian.f(wo, wi, );
        }
    }
    return endColor;
}
