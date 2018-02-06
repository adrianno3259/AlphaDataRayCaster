#include "../include/Grid.h"
#include <cstdlib>

Grid::Grid()
{
    //gridVoxels.push_back(std::vector<Triangle*>());
    //gridVoxels.push_back(std::vector<Triangle*>());
}

/**
*   Initializing a Grid with a full scene from
*   a given pointer to that scene
*/
Grid::Grid(std::shared_ptr<Scene> scene)
{
    this->scene = scene;
}

/**
*   Sets the scene of the Grid afterwards
*/
void Grid::setScene(std::shared_ptr<Scene> scene)
{
    this->scene = scene;
}

/**
*   Aligns every triangle of every voxel in an array of floats
*   and creates an array of integers to keep the cumulative
*   size in number of triangles of each voxel (the size of
*   the voxel plus the size of the previous voxels).
*
*   For example, if the Grid follows this setup:
*
*   Grid
*     |
*   Voxel_0 -> t1 -> t2 -> t3   |   3 elements
*   Voxel_1 -> t0 -> t1 -> t3   |   3 elements
*   Voxel_2 -> t2               |   1 element
*   -------------------------------------------
*   Total                       |   7 elements
*
*   The resulting arrays for this function would be:
*
*   Triangle Attributes (tData):
*      |---- 12 elements ----|
*   {
*     t1.p1.x, ..., t1.normal.z, t2.p1.x, ..., t2.normal.z, t3.p1.x, ..., t3.normal.z,  (Voxel 0)
*     t0.p1.x, ..., t0.normal.z, t1.p1.x, ..., t1.normal.z, t3.p1.x, ..., t3.normal.z,  (Voxel 1)
*     t2.p1.x, ..., t2.normal.z                                                         (Voxel 2)
*   }
*
*   Voxel Triangle Count (vCount):
*
*   { 3, 6, 7 } -> So Voxel 0 have 3 triangles, Voxel 1 plus the previous have 6,
*                  and Voxel 2 and the previous ones sum up to 7
*
*   Returns a pair containing a pointer to tData and vCount respectively
*/
GridData Grid::exportScene() const
{
    int numVoxels = this->gridVoxels.size();
    int numTriangles = 0;
    int *vCount, *idData;
    double *tData;

    /// Allocating the
    vCount = (int*) malloc(sizeof(int)*numVoxels);

    /// Counting the total number of triangles and the cumulative size of each voxel
    for(int i = 0; i < numVoxels; i++)
        numTriangles += this->gridVoxels[i].size(),
        vCount[i] = numTriangles;

    /// Allocating space for every attribute of every Triangle in every Voxel w/ repetition
    tData  = (double*) malloc(sizeof(double)*numTriangles*Triangle::NUM_ATTRIBUTES);

    /// Allocating space for every id of every Triangle in every Voxel w/ repetition
    idData = (int*) malloc(sizeof(int)*numTriangles);


    /// For every Voxel in the Grid
    for(int vox = 0; vox < numVoxels; vox++)
    {
        /// Getting the size of Voxel i
        int voxMax = gridVoxels[vox].size();

        /**
        *   Getting the first index that belongs to Voxel i:
        *   If the voxel number is 0, the base index is 0
        *   Otherwise, the index is the cumulative size of the previous voxel
        */
        int voxelBaseIdx = (vox > 0 ? vCount[vox-1] : 0);

        /// For every Triangle in the Voxel
        for(int tri = 0; tri < voxMax; tri++)
        {
            int triangleBaseIdx = voxelBaseIdx + tri*Triangle::NUM_ATTRIBUTES;

            /// First point
            tData[triangleBaseIdx]    = gridVoxels[vox][tri]->p1.x;
            tData[triangleBaseIdx+1]  = gridVoxels[vox][tri]->p1.y;
            tData[triangleBaseIdx+2]  = gridVoxels[vox][tri]->p1.z;

            /// Second point
            tData[triangleBaseIdx+3]  = gridVoxels[vox][tri]->p2.x;
            tData[triangleBaseIdx+4]  = gridVoxels[vox][tri]->p2.y;
            tData[triangleBaseIdx+5]  = gridVoxels[vox][tri]->p2.z;

            /// Third point
            tData[triangleBaseIdx+6]  = gridVoxels[vox][tri]->p3.x;
            tData[triangleBaseIdx+7]  = gridVoxels[vox][tri]->p3.y;
            tData[triangleBaseIdx+8]  = gridVoxels[vox][tri]->p3.z;

            /// Normal vector
            tData[triangleBaseIdx+9]  = gridVoxels[vox][tri]->normal.x;
            tData[triangleBaseIdx+10] = gridVoxels[vox][tri]->normal.y;
            tData[triangleBaseIdx+11] = gridVoxels[vox][tri]->normal.z;

            /// Triangle Id
            idData[tri] = gridVoxels[vox][tri]->getId();

        }
    }

    GridData ret = GridData(idData, vCount, tData);
    ret.idDataSize  = numTriangles;
    ret.triDataSize = numTriangles*Triangle::NUM_ATTRIBUTES;
    ret.vCountSize  = numVoxels;
    return ret;
}

void Grid::buildGrid()
{

    gridVoxels.push_back(std::vector<std::shared_ptr<Triangle> >());
    int nMeshes = scene->sceneSize();
    //const std::vector<Mesh*> sc = scene->getMeshes();

    for(int m = 0; m < nMeshes; m++)
    {
        std::shared_ptr<Mesh> mesh = scene->getMesh(m);
        int nTris = mesh->triangles.size();
        for(int tri = 0; tri < nTris; tri++)
        {
            gridVoxels[0].push_back(mesh->triangles[tri]);
        }
    }
}

Grid::~Grid()
{
    //dtor
}
