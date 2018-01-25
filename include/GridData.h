#ifndef GRIDDATA_H
#define GRIDDATA_H

/**
*   Exported triangle data from the class Grid
*/
class GridData{
public:
    /// Id information in order of voxel number and
    /// position in voxel
    int *IdData;

    /// Cumulative size of each Voxel
    int *vCount;

    /// triangle information
    float *triData;

    /// Size of IdData Array
    int idDataSize;

    /// Size of triData Array
    int triDataSize;

    /// Size of vCount Array
    int vCountSize;

    /**
    *   Constructor
    *   Sets the pointer to allocated id info and
    *   triangle info
    */
    GridData(int*, int*, float*);

    /**
    *   Free all array pointers
    */
    void freeAll();

    /**
    *   Destructor
    *   Deallocates memory of the used pointers
    */
    ~GridData();
};

#endif // GRIDDATA_H
