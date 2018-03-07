#ifndef SCENE_H
#define SCENE_H

#include "Mesh.h"
#include <vector>
#include <memory>

/**
*   class Scene
*   Stores a whole scene with 3d models and (in the future) light sources
*/
class Scene
{
    public:
        /**
        *   Blank constructor
        *   Initializes an empty scene
        */
        Scene();

        /**
        * Empty destructor
        */
        virtual ~Scene();

        /**
        *   Gets a reference to a vector containing
        *   all objects in the scene
        */
        const std::vector<std::shared_ptr<Mesh> >& getMeshes() const;

        /**
        *   Adds a new object obj to the
        *   existing scene
        */
        void addObject(std::shared_ptr<Mesh> obj);

        /**
        *   Reads mesh from file
        */
        void importObject(const std::string& filename);

        /**
        *   Gets a smart pointer to the mesh at index "index"
        */
        std::shared_ptr<Mesh> getMesh(int index) const;

        /**
        *   Gets the number of meshes in the scene
        */
        int sceneSize() const;

    protected:
        /// Objects in the scene
        std::vector<std::shared_ptr<Mesh> > objects;
    private:
};

#endif // SCENE_H
