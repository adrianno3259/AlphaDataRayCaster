#include <iostream>
#include <cstdlib>

#include <utility>
#include <ctime>
#include <cmath>

#include "include/Vec3d.h"
#include "include/Mesh.h"

#include "include/GridData.h"
#include "include/Triangle.h"
#include "include/Grid.h"

#include "include/Camera.h"
#include "include/CameraData.h"


#include "Image.h"

#include <algorithm>
#include <random>

#include "Light.h"
//#include "include/TriangleBase.h"

#define PV(A) cout<<#A<<" = "<<A<endl
#define FOR(I,N) for(int I = 0; I < N; I++)

#define INV_PI 0.318309886

/// initializing the triangle instance counter
//int Triangle::TRIANGLE_NEXT_ID = 0;
int Mesh::MESH_NEXT_ID = 0;

using namespace std;

#define forObj(mesh) cout<<"mesh id = "<<mesh->getId()<<"\n";for(int i = 0; i < 5; i++){mesh->triangles[i]->printData();}

vector<shared_ptr<Mesh> > meshes;
/*
vector<shared_ptr<Light> >    lights;
vector<shared_ptr<Material> > materials;
*/

int main(int argc, char** argv){

    string obj, output;
    int camx = 4, camy = 0, camz = 3;
    int hres = atoi(argv[3]), vres = atoi(argv[4]);
    double psize = 1.0;
    if(argc > 1 )
    {
        obj = argv[1];
        output = argv[2];
        camx = atof(argv[5]);
        camy = atof(argv[6]);
        camz = atof(argv[7]);
        psize = atof(argv[8]);
    }
    else
    {
        obj = "3d_models/teddy.obj";
        output = "image.ppm";
    }


    Vec3d eye(camx, camy, camz), lkp(0, 0, 0), up(0,0,1);
    double dist = 200;

    Camera cam = Camera(eye, lkp, up, dist, psize, vres, hres);

    shared_ptr<Mesh> m = make_shared<Mesh>(obj);

    shared_ptr<Image> im = make_shared<Image>(hres, vres);

    shared_ptr<Light> light = make_shared<Light>(2.0, Color(1.0), Vec3d(50.0, 50.0, 50.0));

    Color matCol = RED;
    double matDif = 0.7;

    struct timespec start, stop;

    clock_gettime(CLOCK_MONOTONIC, &start);//time = clock();

    int r, c;
    for(r = 0; r < vres; r++)
    for(c = 0; c < hres; c++)
    {
        Color L;
        Ray ray = cam.getRay(r, c);

        double tMin = 10000000.0;
        int idMin = -1, iMin = -1;
        for(int i = 0; i < m->triangles.size(); i++)
        {
            double t;
            bool hit = m->triangles[i]->hit(ray, t);
            if(hit && t < tMin)
            {
                idMin = m->triangles[i]->getId();
                tMin = t;
                iMin = i;
            }
        }

        if(idMin != -1)
        {
            Vec3d wo = -ray.direction;
            double dot = m->triangles[iMin]->normal * wo;
            Color lightInfluence = light->mColor * light->mIntensity;
            L = (matCol * matDif * INV_PI) * lightInfluence * dot;
        }

        im->setPixel(c, r, L);

    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    float tm = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
    printf("Rendering %s with resolution %dx%d took %f seconds\n",
            obj.c_str(),
            hres,
            vres,
            tm);

    cout<<"Saving "<<output<<"\n";
    im->save(output.c_str());

    return 0;
}
