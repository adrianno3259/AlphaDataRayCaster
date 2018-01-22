#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "Triangle.h"


/**
*   class Mesh
*   Stores a triangle mesh
*/
class Mesh
{
    public:
        /// Vector with all the triangles of the mesh
        std::vector<Triangle*> triangles;

        /**
        *   File constructor
        *   Creates a mesh instance from an .obj file
        */
        Mesh(std::string filename);

        /**
        *   Blank constructor
        *   Creates a mesh with no triangles
        */
        Mesh();

        /**
        *   Adds a triangle to the mesh
        */
        void addTriangle(Triangle* tri);

        /**
        *   Blank destructor
        */
        virtual ~Mesh();
    protected:
    private:
};

#endif // MESH_H
