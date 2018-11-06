#include <material/Material.h>


namespace material
{

    Material::Material() {}

    Material::~Material() {}

    Matte::Matte(double kd, const Color& cd) :
        m_diffuseBRDF(brdf::Lambertian(kd, cd))
    {

    }

    Matte::~Matte()
    {

    }

    Color Matte::shade(const Intersection& inter, const std::vector<Light>& lights) const
    {
        Color L = BLACK;
        for(auto&& light : lights)
        {
            auto wi = light.getDirection(inter.hitPoint);
            auto wo = -inter.rayDirection;
            auto lightInfluence = light.getRadiance();
            double dot = inter.normalVector * wo;
            L += this->m_diffuseBRDF.f(inter, wi, wo) * lightInfluence * dot;
        }

        return L;

    }

}
