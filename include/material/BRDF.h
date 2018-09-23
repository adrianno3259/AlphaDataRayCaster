#ifndef BRDF_H
#define BRDF_H

#include "Color.h"
#include "Vec3d.h"
#include <Intersection.h>


namespace brdf
{

    /** Super-class of every BRDF class
    *   BRDF stands for Bi-directional Reflectance
    *   Distribution Function and is responsible for
    *   modeling the behavior of the light interactions
    *   with a surface of a given object.
    *
    *   This class is the base for the Material classes,
    *   since every material has at least one BRDF instance.
    */
    class BRDF
    {
    public:

        BRDF();

        virtual ~BRDF();

        /** The BRDF function itself as seen in the rendering
        *   equation. Returns the proportion between the
        *   arriving irradiance and the leaving radiance.
        *   This method returns a color to allow the multiplication
        *   with the colors of the arriving lights
        *
        *   @param [in] intersect
        *       Point in which the irradiance is arriving
        *
        *   @param [in] wi
        *       Incident light direction
        *
        *   @param [in] wo
        *       Direction from which the point is being 'seen'.
        *       The reflected light direction
        *
        */
        virtual Color f(const Intersection& intersect, const Vec3d& wi, const Vec3d& wo) const = 0;

    };


    /** Class that models a perfectly diffuse material. Even though
    *   there are no real materials with this behavior, it is sufficiently
    *   good to represent very dull, matte materials.
    *
    */
    class Lambertian : BRDF
    {
    public:
        /** Default constuctor for the lambertian BRDF
        *
        *   @param [in] diffuseCoef
        *       coefficient of the diffuse component of the
        *       BRDF. Can also be called "intensity"
        *
        *   @param [in] color
        *       color that the BRDF will return multiplied
        *       by the intensity. Used to model how the surface
        *       absorb and reflect some colors.
        */
        Lambertian(double diffuseCoef, const Color& cd);

        /** Virtual destructor. No action.
        */
        virtual ~Lambertian();

        /**
        *   @see BRDF::f
        */
        virtual Color f(const Intersection& intersect, const Vec3d& wi, const Vec3d& wo) const override;

    private:
        double m_diffuseCoeficient;
        Color m_color;
    };

}
#endif // BRDF_H
