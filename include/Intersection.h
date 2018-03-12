#ifndef INTERSECTION_H
#define INTERSECTION_H


#include <memory>
#include "Color.h"
#include "Vec3d.h"
#include "Ray.h"
#include "Material.h"
#include "Triangle.h"

#define printInters(A) std::cout <<"Intersection "<< #A <<":"<<std::endl; printVar(A.hit); printVar(A.t); printVec(A.hitPoint); printVar(A.entering); printCol(A.c)


typedef struct Intersection
{
public:

    Intersection(const bool ht,
              const float tnum,
              std::shared_ptr<Triangle> o,
              const bool ent,
              const Vec3d& htp,
              const Vec3d& norm,
              std::shared_ptr<Material> mat)
    :   hit(ht),
        t(tnum),
        obj(o),
        entering(ent),
        hitPoint(htp),
        normal(norm),
        m(mat) {}

    Ray r;
    bool hit;
    float t; // ponto paramétrico t no raio que atingiu o objeto
    std::shared_ptr<Triangle> obj; // objeto atingido
    bool entering; // se o raio da interseção está entrando ou não no objeto
    int depth;
    Vec3d hitPoint;
    Vec3d normal;
    std::shared_ptr<Material> m;
} Intersection;

#endif // INTERSECTION_H
