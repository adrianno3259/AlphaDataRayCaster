#include "CameraData.h"
#include <cstdlib>

CameraData::CameraData(double* rays, int nRays)
{
    this->rayData = rays;
    this->rayDataSize = nRays;
}

void CameraData::freeAll(){
    free(this->rayData);
}

