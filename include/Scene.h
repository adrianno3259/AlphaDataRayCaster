#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Mesh.h"

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
        const std::vector<Mesh*>& getMeshes() const;

        /**
        *   Adds a new object obj to the
        *   existing scene
        */
        void addObject(Mesh* obj);

    protected:
        /// Objects in the scene
        std::vector<Mesh*> objects;
    private:
};

#endif // SCENE_H
