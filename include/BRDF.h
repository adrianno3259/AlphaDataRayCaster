#ifndef BRDF_H
#define BRDF_H


class Intersection;

class BRDF
{
    public:
        BRDF(float diffuseCoef, const Color& color);
        virtual ~BRDF();

        Color f(const Vec3d& wo,
                const Vec3d& wi,
                Intersection& it) const;

    protected:
        double mDiffuseCoef;
        Color  mColor;
    private:
};

#endif // BRDF_H
