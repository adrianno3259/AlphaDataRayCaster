#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vec3d.h"
#include "../Globals.h"
#include "Ray.h"

/**
*   class Triangle
*   Stores the three vertices and the normal of a triangle
*/
class Triangle
{
    public:

        /// Triangle vertices
        Vec3d p1, p2, p3;

       /// Trangle normal vector
        Vec3d normal;

        /**
        *   Blank constructor
        *   Initializes a triangle in:
        *   p1(0,0,0), p2(0,0,1) and p3(1,0,0)
        *   Normal vector equals to (0,1,0)
        */
        Triangle();

        /**
        *   Blank constructor
        *   Initializes a triangle in pt1, pt2 and pt3
        *   With auto-calculated normal vector
        */
        Triangle(Vec3d pt1, Vec3d pt2, Vec3d pt3, int relativeId);

        /**
        *   Blank constructor
        *   Initializes a triangle in pt1, pt2 and pt3
        *   Normal vector equals to normal
        */
        Triangle(Vec3d pt1, Vec3d pt2, Vec3d pt3, Vec3d normal, int relativeId);

        /**
        *   Get triangle id relative to mesh
        */
        int getId() const;

        /**
        *   Get the id of the mesh that the triangle belongs to
        */
        int getMeshId() const;

        /**
        *   Set triangle id
        */
        void setId(int id);

        /**
        *   Print all triangle information:
        *   Vertices, Normal and Id
        */
        void printData() const;

        /**
        *   Checks if Ray hits the triangle triangle in
        *   a positive t value. Otherwise returns -1
        */
        bool hit(const Ray& ray, double& t) const;

        /**
        *   Triangle Destructor
        *   Decrements TRIANGLE_NEXT_ID
        */
        virtual ~Triangle();

        /// Id number for the next instance of Triangle
        static int TRIANGLE_NEXT_ID;

        /// Number of attributes to be stored and sent
        /// to the FPGA
        static const int NUM_ATTRIBUTES = 12;

    protected:
        /// Triangle id
        int id;

        /// id of the mesh the triangle is in.
        /// easier to find the correct material
        int meshId;
    private:

};

#endif // TRIANGLE_H
