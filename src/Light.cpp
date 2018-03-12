#include "Light.h"

Light::Light()
{

}

Light::Light(float intensity,
      Color color,
      Vec3d position) :

    mIntensity(intensity),
    mColor(color),
    mPosition(position)
{


}


Light::~Light()
{
    //dtor
}


float Light::getIntensity() const
{
    return this->mIntensity;
}

void Light::setIntensity(float intensity)
{
    this->mIntensity = intensity;
}

Color Light::getColor() const
{
    return this->mColor;
}

void Light::setColor(const Color& color)
{
    this->mColor = color;
}

Vec3d Light::getPosition() const
{
    return this->mPosition;
}

void Light::setPosition(const Vec3d& position)
{
    this->mPosition = position;
}

Vec3d Light::getDirection(const Vec3d& point) const
{
    return (this->mPosition - point).hat();
}

Color Light::getL() const
{
    return this->mColor * this->mIntensity;
}
