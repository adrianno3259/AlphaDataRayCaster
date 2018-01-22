#include <iostream>

#include "include/Vec3d.h"
#include "include/Mesh.h"

#include "include/GridData.h"
#include "include/Triangle.h"
#include "include/Grid.h"

/// initializing the triangle instance counter
int Triangle::TRIANGLE_NEXT_ID = 0;

void init(){



}

using namespace std;

int main(){

    init();

    Mesh m("3d_models/teddy.obj");
    Mesh m2("3d_models/teddy.obj");
    for(int i = 0; i < 3; i++)
    {
        m.triangles[i]->printData();
        m2.triangles[i]->printData();
    }

    Scene s = Scene();
    s.addObject(&m2);


    vector<Mesh*> meshes = s.getMeshes();
    cout<<"scene time! :)\n";
    for(int i = 0; i < 3; i++)
    {
        meshes[0]->triangles[i]->printData();
    }

    Grid g1 = Grid();
    g1.gridVoxels[0].push_back(meshes[0]->triangles[0]);
    g1.gridVoxels[1].push_back(meshes[0]->triangles[1]);
    GridData gd = g1.exportScene();

    for(int i = 0; i < gd.idDataSize; i++)
        cout<<"Id === "<<gd.IdData[i]<<endl;
    cout<<endl;

    for(int i = 0; i < gd.triDataSize; i++)
        cout<<gd.triData[i]<<" ";
    cout<<endl;

    for(int i = 0; i < gd.vCountSize; i++)
        cout<<gd.vCount[i]<<" ";
    cout<<endl;


    gd.freeAll();

    return 0;
}
