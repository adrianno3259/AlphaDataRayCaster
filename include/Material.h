#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"
#include "Ray.h"
#include "Vec3d.h"
#include "Triangle.h"
#include "Light.h"
#include <vector>
#include <memory>
#include "BRDF.h"

class Intersection;

class Material
{
    public:
        Material(float diffuseCoef, const Color& color);

        virtual ~Material();

        Color shade(Intersection& it,
                    const std::vector<std::shared_ptr<Light> >& lights) const;


    protected:
        BRDF lambertian;
    private:

};

#endif // MATERIAL_H
