#include "material/BRDF.h"
#include <Constants.h>

namespace brdf
{
    /// Empty BRDF constructor and destructor
    BRDF::BRDF() {}

    BRDF::~BRDF(){}

    /// Lambertian BRDF implementations
    Lambertian::Lambertian(double diffuseCoef, const Color& cd) :
        m_diffuseCoeficient(diffuseCoef),
        m_color(cd)
    {

    }

    Lambertian::~Lambertian(){}

    Color Lambertian::f(const Intersection& intersect, const Vec3d& wi, const Vec3d& wo) const
    {
        return this->m_color * this->m_diffuseCoeficient * constants::INV_PI;
    }

}
