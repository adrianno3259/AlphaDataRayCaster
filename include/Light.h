#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Vec3d.h"

class Light
{
    public:
        Light();
        Light(float intensity,
              Color color,
              Vec3d position);

        virtual ~Light();

        float mIntensity;
        Color mColor;
        Vec3d mPosition;

    private:
};

#endif // LIGHT_H
