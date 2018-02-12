#include "TriangleBase.h"

TriangleBase::TriangleBase(){}

void TriangleBase::addTriangle(std::shared_ptr<Triangle> triangle)
{
    //this->triangles.push_back(triangle);
}

void TriangleBase::addTriangle(const Vec3d& p1, const Vec3d& p2, const Vec3d& p3)
{
    //this->triangles.push_back(std::make_shared<Triangle>(p1, p2, p3,1));
}

std::shared_ptr<Triangle> TriangleBase::getTriangle(int id) const
{
    return this->triangles.at(id);
}
