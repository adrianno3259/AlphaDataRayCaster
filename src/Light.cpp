#include "Light.h"

Light::Light()
{

}

Light::Light(double intensity, const Color& color, const Vec3d& position) :
    mIntensity(intensity),
    mColor(color),
    mPosition(position)
{


}


Light::~Light() {}


Color Light::getRadiance() const
{
    return this->mIntensity * this->mColor;
}

Vec3d Light::getDirection(const Vec3d& inter) const
{
    return this->mPosition - inter;
}
