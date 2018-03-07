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
