#include <iostream>
#include <cstdlib>

#include <utility>

#include <cmath>

#include "include/Vec3d.h"
#include "include/Mesh.h"

#include "include/GridData.h"
#include "include/Triangle.h"
#include "include/Grid.h"

#include "include/Camera.h"
#include "include/CameraData.h"

#include "Intersection.h"
#include "Material.h"
#include "Image.h"

#include <algorithm>
#include <random>

#include "Light.h"
//#include "include/TriangleBase.h"

#define PV(A) cout<<#A<<" = "<<A<endl
#define FOR(I,N) for(int I = 0; I < N; I++)

/// initializing the triangle instance counter
//int Triangle::TRIANGLE_NEXT_ID = 0;
int Mesh::MESH_NEXT_ID = 0;

using namespace std;

#define forObj(mesh) cout<<"mesh id = "<<mesh->getId()<<"\n";for(int i = 0; i < 5; i++){mesh->triangles[i]->printData();}

int main(){

    Vec3d eye(50, 50, 0), lkp(0, 0, 0), up(0,0,1);
    double dist = 100, psize=1;
    int vres = 300, hres = 300;
    Camera cam = Camera(eye, lkp, up, dist, psize, vres, hres);



    shared_ptr<Mesh> m = make_shared<Mesh>("3d_models/teddy.obj");

    shared_ptr<Image> im = make_shared<Image>(vres, hres);

    shared_ptr<Light> light = make_shared<Light>(1.0, Color(1.0), Vec3d(10.0, 10.0, 10.0));
    vector<shared_ptr<Light> > lights; lights.push_back(light);

    shared_ptr<Material> mat = make_shared<Material>(1.0, Color(1.0, 0.0, 0.0));

    for(int r = 0; r < vres; r++)
    for(int c = 0; c < hres; c++)
    {
        Color L;
        Ray ray = cam.getRay(r, c);
        double tMin = 10000000.0;
        int idMin = -1;
        int inMeshId = -1;
        bool hit;

        for(int i = 0; i < m->triangles.size(); i++)
        {
            double t;
            hit = m->triangles[i]->hit(ray, t);
            if(hit && t < tMin)
            {
                inMeshId = i;
                idMin = m->triangles[i]->getId();
                tMin = t;
            }
        }
        /*
        Intersect it = Intersect(hit,
                                 tMin,
                                 m->triangles[inMeshId],
                                 true,
                                 ray.rayPoint(tMin),
                                 m->triangles[inMeshId]->normal,
                                 mat);


        if(inMeshId != -1)
            L = mat->shade(it, lights);
        else*/
            L = Color(1.0);
        im->setPixel(r, c, L);

    }

    im->save("image.ppm");

    return 0;
}
