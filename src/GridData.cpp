#include "GridData.h"
#include <iostream>
#include <cstdlib>

GridData::GridData(int* ids, int* cumulativeSizes, double* triangles)
{
    this->IdData = ids;
    this->vCount = cumulativeSizes;
    this->triData = triangles;
}

void GridData::freeAll(){
    free(this->IdData);
    free(this->vCount);
    free(this->triData);
}

GridData::~GridData()
{
    std::cout<<"destrutor teste\n\n----------\n\n";
}
