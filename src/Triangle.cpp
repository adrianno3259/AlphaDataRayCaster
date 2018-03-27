#include "../include/Triangle.h"
#include "../Globals.h"
#include <iostream>

#define P(A) std::cout<<#A<<" = "<<A<<std::cout

//int Triangle::TRIANGLE_NEXT_ID = 0;

Triangle::Triangle()  :
    p1(Vec3d(-0.5,0,-0.5)),
    p2(Vec3d(0,0,1)),
    p3(Vec3d(1,0,0))
{
    normal = Vec3d(0,1,0);
}

Triangle::Triangle(Vec3d pt1, Vec3d pt2, Vec3d pt3, int relativeId) :
    p1(pt1), p2(pt2), p3(pt3), id(relativeId)
{
    normal = (p2 - p1) ^ (p3 - p1);
    normal.normalize();
}

Triangle::Triangle(Vec3d pt1, Vec3d pt2, Vec3d pt3, Vec3d normal, int relativeId) :
    p1(pt1), p2(pt2), p3(pt3), normal(normal), id(relativeId)
{ }

Triangle::~Triangle(){}

int Triangle::getId() const
{
    return id;
}

void Triangle::setId(int id)
{
    this->id = id;
}

void Triangle::setMeshId(int id)
{
    this->meshId = id;
}

void Triangle::printData() const
{
    std::cout<<"Triangulo: "<<getId()<<std::endl;
    printVec(p1);
    printVec(p2);
    printVec(p3);
    printVec(normal);
}

bool Triangle::hit(const Ray& ray, double& t) const
{
    bool hit = false;

    double a = p1.x - p2.x, b = p1.x - p3.x, c = ray.direction.x, d = p1.x - ray.origin.x;
    double e = p1.y - p2.y, f = p1.y - p3.y, g = ray.direction.y, h = p1.y - ray.origin.y;
    double i = p1.z - p2.z, j = p1.z - p3.z, k = ray.direction.z, l = p1.z - ray.origin.z;

    double m = f*k - g*j, n = h*k - g*l, p = f*l - h*j;
    double q = g*i - e*k, s = e*j - f*i;

    double inv_denom = 1.0/(a*m + b*q + c*s);

    double e1 = d*m - b*n - c*p;
    double beta = e1*inv_denom;

    if(beta < 0.0)      return false;

    double r = e*l - h*i;
    double e2 = a*n + d*q + c*r;
    double gamma = e2 * inv_denom;

    if(gamma < 0.0)         return false;
    if(beta + gamma > 1.0)  return false;

    double e3 = a*p - b*r + d*s;
    double tmin = e3 * inv_denom;

    if(tmin < K_EPSILON)    return false;

    t = tmin;

    return true;

}
