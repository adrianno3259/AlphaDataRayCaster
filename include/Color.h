#ifndef COLOR_H
#define COLOR_H

#define printCol(A) std::cout << #A << " = (" << (A).r <<", "<<(A).g<<", "<<(A).b<<")"<<std::endl

#define BLACK Color(0.0)
#define WHITE Color(1.0)
#define RED Color(1.0, 0.0, 0.0)
#define GREEN Color(0.0, 1.0, 0.0)
#define BLUE Color(0.0, 0.0, 1.0)
#define YELLOW Color(1.0, 1.0, 0.0)
#define MAGENTA Color(0.0, 1.0, 1.0)
#define ORANGE Color(1.0, 0.5, 0.0)


class Color
{
public:
    float r, g, b;
    Color(): r(0.0), g(0.0), b(0.0) {} // KAIOSHIN
    Color(const float a) : r(a), g(a), b(a) {} // KAIOSHIN
    Color(const float a, const float b, const float c) : r(a), g(b), b(c) {} // KAIOSHIN
    Color(const Color& v) : r(v.r), g(v.g), b(v.b) {} // KAIOSHIN
    ~Color (void) {} // HAKAISHIN
    Color& operator= (const Color& rhs); //Sobrecarrega os operadores de atribuição
    Color operator- (void) const; // Operador menos unário
    Color operator* (const float a) const; //multiplicação por escalar
    Color operator/ (const float a) const; // divisão por escalar
    Color operator+ (const Color& v) const; // soma de cores
    Color& operator+= (const Color& v); // soma e atribuição de cores
    Color operator- (const Color& v) const; // subtração de cores
    Color operator* (const Color& b) const; // produto de cores
    Color operator/ (const Color& b) const;
    Color clamp();
};

// ----- multiplicação com o escalar do lado esquerdo -------
Color operator* (const double a, const Color& v);
Color operator* (const int a, const Color& v);



/********************** Implementaçoes Inline *************************/

inline Color Color::operator- (void) const { return Color(-r, -g, -b); }
inline Color Color::operator* (const float a) const { return Color(a*r, a*g, a*b); }
inline Color Color::operator/ (const float a) const { return Color(r/a, g/a, b/a); }
inline Color Color::operator+ (const Color& c) const { return Color(r + c.r, g + c.g, b + c.b); }
inline Color& Color::operator+= (const Color& c) { r += c.r; g += c.g; b += c.b; return (*this); }
inline Color Color::operator- (const Color& c) const { return Color(r - c.r, g - c.g, b - c.b); }
inline Color Color::operator* (const Color& c) const { return Color(r * c.r, g * c.g, b * c.b); }
inline Color Color::operator/ (const Color& c) const { return Color(r / c.r, g / c.g, b / c.b); }
inline Color operator* (const double a, const Color& v) { return (Color(a * v.r, a * v.g, a * v.b)); }
inline Color operator* (const int a, const Color& v) { return (Color(a * v.r, a * v.g, a * v.b)); }


#endif // COLOR_H
