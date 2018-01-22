#ifndef GRID_H
#define GRID_H

#include <vector>
#include <utility>

#include "Scene.h"
#include "GridData.h"


class Grid
{

    public:
        /**
        *   Blank constructor
        *   Initializes an empty grid
        */
        Grid();

        /**
        *   Scene constructor
        *   Initializes a Grid with a full scene
        */
        Grid(Scene* scene);

        /**
        *   Sets the scene of the Grid afterwards
        */
        void setScene(Scene* scene);

        /**
        *   Export the built Grid for processing
        *   in the FPGA
        */
        GridData exportScene() const;

        virtual ~Grid();

        /**
        *   Two-Dimensional vector containing the full Grid setup
        *   The first index is a Voxel and the second is all the
        *   Triangles that belongs to it
        */
        std::vector<std::vector<Triangle*> > gridVoxels;
    protected:


        /// Scene from which the Grid will be built
        Scene* scene;
};


#endif // GRID_H
