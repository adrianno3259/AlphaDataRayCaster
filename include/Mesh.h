#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "Triangle.h"
#include <memory>

/**
*   class Mesh
*   Stores a triangle mesh
*/
class Mesh
{
    public:
        /// Vector with all the triangles of the mesh
        std::vector<std::shared_ptr<Triangle> > triangles;

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
        void addTriangle(std::shared_ptr<Triangle> tri);

        /**
        *   Blank destructor
        */
        virtual ~Mesh();

        /// Return the mesh id
        int getId() const;

        //static int MESH_NEXT_ID;

    protected:
        int id;
    private:
};

#endif // MESH_H
