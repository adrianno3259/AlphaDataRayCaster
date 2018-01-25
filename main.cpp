#include <iostream>

#include "include/Vec3d.h"
#include "include/Mesh.h"

#include "include/GridData.h"
#include "include/Triangle.h"
#include "include/Grid.h"

#include "include/Camera.h"
#include "include/CameraData.h"

#define PV(A) cout<<#A<<" = "<<A<endl

/// initializing the triangle instance counter
int Triangle::TRIANGLE_NEXT_ID = 0;

void init(){



}

using namespace std;

int main(){

    init();

    Vec3d eye(-100,0,0), lkp(0,0,0), up(0,1,0);
    double dist = 50, psize=0.5;
    int vres = 1, hres = 1;
    Camera cam = Camera(eye, lkp, up, dist, psize, vres, hres);
    Ray r = cam.getRay(0,0);
    printVec(r.origin);
    printVec(r.direction);


    /*
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
    */
    return 0;
}
