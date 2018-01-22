#include "../include/Triangle.h"
#include <iostream>

#define P(A) std::cout<<#A<<" = "<<A<<std::cout

//int Triangle::TRIANGLE_NEXT_ID = 0;

Triangle::Triangle()  :
    p1(Vec3d(0,0,0)),
    p2(Vec3d(0,0,1)),
    p3(Vec3d(1,0,0))
{
    normal = Vec3d(0,1,0);
    id = TRIANGLE_NEXT_ID;
    TRIANGLE_NEXT_ID++;
}

Triangle::Triangle( Vec3d pt1, Vec3d pt2, Vec3d pt3) :
    p1(pt1), p2(pt2), p3(pt3)
{
    normal = (p2 - p1) ^ (p3 - p1);
    id = TRIANGLE_NEXT_ID;
    Triangle::TRIANGLE_NEXT_ID++;
}

Triangle::Triangle(Vec3d pt1, Vec3d pt2, Vec3d pt3, Vec3d normal) :
    p1(pt1), p2(pt2), p3(pt3), normal(normal)
{
    id = TRIANGLE_NEXT_ID;
    TRIANGLE_NEXT_ID++;
}

Triangle::~Triangle(){}

int Triangle::getId() const
{
    return id;
}

void Triangle::setId(int id)
{
    this->id = id;
}

void Triangle::printData() const
{
    std::cout<<"Triangulo: "<<getId()<<std::endl;
    printVec(p1);
    printVec(p2);
    printVec(p3);
    printVec(normal);
}
