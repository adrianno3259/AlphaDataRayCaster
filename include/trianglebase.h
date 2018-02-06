#ifndef TRIANGLEBASE_H
#define TRIANGLEBASE_H

#include "Triangle.h"
#include <memory>
#include <vector>

/**
    TriangleBase: Database for quick access of Triangles only by an ID value
        - Useful for searches
        - Other objects that contains triangles will store a pointer
*/
class TriangleBase
{
    public:
        /*
        *   Empty Constructor and Destructor
        */
        TriangleBase();

        /**
        *   Adds a new triangle to the end of the database
        */
        void addTriangle(std::shared_ptr<Triangle> triangle);

        /**
        *   Creates and adds a new triangle from 3 given points
        */
        void addTriangle(const Vec3d& p1, const Vec3d& p2, const Vec3d& p3);

        /**
        *   Gets a triangle from the id number
        */
        std::shared_ptr<Triangle> getTriangle(int id) const;

    protected:
        std::vector<std::shared_ptr<Triangle> > triangles;
    private:
};

#endif // TRIANGLEBASE_H
