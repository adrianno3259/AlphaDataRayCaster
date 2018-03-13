#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"
#include "Ray.h"
#include "Vec3d.h"
#include "Triangle.h"
#include "Light.h"
#include <vector>
#include <memory>

class Intersection;

class Material
{
    public:
        Material(float diffuseCoef, Color color);

        virtual ~Material();

        Color shade(Intersection& it,
                    const std::vector<std::shared_ptr<Light> >& lights) const;

        double mDiffuseCoef;
        Color  mColor;
    protected:

    private:

};

#endif // MATERIAL_H
