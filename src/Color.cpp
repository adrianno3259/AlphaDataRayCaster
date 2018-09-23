#include <algorithm>
#include "Color.h"


Color& Color::operator= (const Color& rhs){
    if (this == &rhs)
        return (*this);
    r = rhs.r; g = rhs.g; b = rhs.b;
    return (*this);
}



Color& Color::clamp()
{
    float maxVal = std::max(r, std::max(g, b));

    if(maxVal > 1.0)
    {
        r /= maxVal;
        g /= maxVal;
        b /= maxVal;

    }
    return *this;
}



