#include <iostream>

#include "include/Vec3d.h"
#include "include/Mesh.h"

#include "include/GridData.h"
#include "include/Triangle.h"
#include "include/Grid.h"

#define PV(A) cout<<#A<<" = "<<A<endl

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
        //m.triangles[i]->printData();
        //m2.triangles[i]->printData();
    }

    Scene s = Scene();
    s.addObject(&m2);


    const vector<Mesh*> meshes = s.getMeshes();
    cout<<"scene time! :)\n";
    for(int i = 0; i < 3; i++)
    {
        meshes[0]->triangles[i]->printData();
    }

    Grid g1 = Grid(&s);

    g1.buildGrid();

    GridData gd = g1.exportScene();


    gd.freeAll();

    return 0;
}
