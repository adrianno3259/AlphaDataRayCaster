#ifndef MATERIAL_H
#define MATERIAL_H

#include "BRDF.h"
#include <vector>
#include <Light.h>
#include <Color.h>
#include <Intersection.h>


namespace material
{

    /** Super-class of all Material types used by the ray tracer.
    *   Each material type contains one or more BRDF instances,
    *   that are responsible for modeling the light behavior.
    */
    class Material{
    public:
        Material();
        virtual ~Material();
        virtual Color shade(const Intersection& inter, const std::vector<Light>& lights) const = 0;
    private:

    };

    class Matte : public Material{
    public:
        Matte(double diffuseCoeficient, const Color& diffuseColor);
        virtual ~Matte();
        virtual Color shade(const Intersection& inter, const std::vector<Light>& lights) const override;
    protected:
        brdf::Lambertian m_diffuseBRDF;
    };


}



#endif // MATERIAL_H
