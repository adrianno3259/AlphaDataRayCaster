#ifndef VEC3_H
#define VEC3_H

#define printVec(A) std::cout << #A << " = (" << (A).x <<", "<<(A).y<<", "<<(A).z<<")"<<std::endl

class Vec3d
{
public:
    /// x-, y- and z-coordinates
    float x, y, z;

    /**
    *   Blank constructor
    *   Initializes a Vector in (0,0,0)
    */
    Vec3d(): x(0.0), y(0.0), z(0.0) {} // KAIOSHIN

    /**
    *   Unit constructor
    *   Initializes a Vector with the same value
    *   for x, y, and z-coordinates
    */
    Vec3d(const float a) : x(a), y(a), z(a) {} // KAIOSHIN

    /**
    *   Full constructor
    *   Initializes a Vector in (a,b,c)
    */
    Vec3d(const float a, const float b, const float c) : x(a), y(b), z(c) {} // KAIOSHIN

    /**
    *   Copy constructor
    *   Initializes a Vector from an existing one
    */
    Vec3d(const Vec3d& v) : x(v.x), y(v.y), z(v.z) {} // KAIOSHIN

    /**
    *   Blank destructor
    */
    ~Vec3d (void){};

    /**
    *   Assignment operator
    */ // HAKAISHIN
    Vec3d& operator= (const Vec3d& rhs);

    /**
    *   Unary minus operator
    */
    Vec3d operator- (void) const;

    /**
    *   Vector times scalar operator
    */
    Vec3d operator* (const float a) const;

    /**
    *   Vector divided by scalar operator
    */
    Vec3d operator/ (const float a) const;

    /**
    *   Vector sum operator
    */
    Vec3d operator+ (const Vec3d& v) const;

    /**
    *   Vector sum and attribution
    */
    Vec3d& operator+= (const Vec3d& v);

    /**
    *   Vector subtraction
    */
    Vec3d operator- (const Vec3d& v) const;

    /**
    *   Dot product of vectors
    *   Returns a float
    */
    float operator* (const Vec3d& b) const; // produto escalar - dot product de dois vetores

    /**
    *   Cross product of two vectors
    *   Returns a perpendicular Vector
    */
    Vec3d operator^ (const Vec3d& v) const;

    /**
    *   Returns the length of a Vector
    */
    float length(void);

    /**
    *   Returns the squared length of a vector
    *   Same as 'length' but without the sqrt
    */
    float len_squared(void);

    /**
    *   Normalize a vector
    *   Divides each coordinate of the current
    *   vector by the length
    */
    void normalize(void);

    /**
    *   Returns a normalized vector
    *   Don't alter the original vector
    */
    Vec3d& hat(void); // retorna o vetor normalizado e transforma o vetor em unitário
};

// ----- multiplicação com o escalar do lado esquerdo -------
/**
*   Vector times float from the left
*/
Vec3d operator* (const double a, const Vec3d& v);

/**
*   Vector times int from the left
*/
Vec3d operator* (const int a, const Vec3d& v);



/**
*   Inline implementations
*/

inline Vec3d Vec3d::operator- (void) const { return Vec3d(-x, -y, -z); }
inline Vec3d Vec3d::operator* (const float a) const { return Vec3d(a*x, a*y, a*z); }
inline Vec3d Vec3d::operator/ (const float a) const { return Vec3d(x/a, y/a, z/a); }
inline Vec3d Vec3d::operator+ (const Vec3d& c) const { return Vec3d(x + c.x, y + c.y, z + c.z); }
inline Vec3d& Vec3d::operator+= (const Vec3d& c) { x += c.x; y += c.y; z += c.z; return (*this); }
inline Vec3d Vec3d::operator- (const Vec3d& c) const { return Vec3d(x - c.x, y - c.y, z - c.z); }
inline float Vec3d::operator* (const Vec3d& c) const { return ( (x * c.x)+ (y * c.y) + (z * c.z) ) ; }
inline Vec3d Vec3d::operator^ (const Vec3d& v) const { return Vec3d(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

inline Vec3d operator* (const double a, const Vec3d& v) { return (Vec3d(a * v.x, a * v.y, a * v.z)); }
inline Vec3d operator* (const int a, const Vec3d& v) { return (Vec3d(a * v.x, a * v.y, a * v.z)); }



#endif // VEC3_H
