#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Vec3d.h"
#include <Intersection.h>

/** Super-class for all types of light sources. It stores
*   information like, light intensity, color and position.
*   It also contains methods to obtain the light surface
*   leaving radiance.
*
*   P.S.: Currently just modeling a point light
*/
class Light
{
    public:
        Light();
        Light(float intensity,
              const Color& color,
              const Vec3d& position);

        virtual Color getRadiance() const;
        virtual Vec3d getDirection(const Vec3d& inter) const;

        virtual ~Light();

        float mIntensity;
        Color mColor;
        Vec3d mPosition;

    private:
};

#endif // LIGHT_H
