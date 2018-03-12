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

    /** getter and setter of the intensity value
    */
    float getIntensity() const;
    void setIntensity(float intensity);

    /** getter and setter of the color value
    */
    Color getColor() const;
    void setColor(const Color& color);

    /** getter and setter of the position value
    */
    Vec3d getPosition() const;
    void setPosition(const Vec3d& position);

    /** returns a vector  of the direction from
    *
    */
    Vec3d getDirection(const Vec3d& point) const;


    /** returns the light irradiance, which is
    *   the color of the lamp times the intensity
    */
    Color getL() const;

private:
    float mIntensity;
    Color mColor;
    Vec3d mPosition;

};

#endif // LIGHT_H
