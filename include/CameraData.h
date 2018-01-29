#ifndef CAMERADATA_H
#define CAMERADATA_H


class CameraData
{
public:
    /// ray information
    double *rayData;

    /// Size of rayData Array
    int rayDataSize;

    /**
    *   Constructor
    *   Sets the pointer to allocated ray info
    */
    CameraData(double*, int);

    /**
    *   Free array pointer
    */
    void freeAll();


};

#endif // CAMERADATA_H
