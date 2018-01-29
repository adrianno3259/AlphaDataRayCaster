#include <iostream>
#include <cstdlib>

#include <utility>

#include "include/Vec3d.h"
#include "include/Mesh.h"

#include "include/GridData.h"
#include "include/Triangle.h"
#include "include/Grid.h"

#include "include/Camera.h"
#include "include/CameraData.h"

#define PV(A) cout<<#A<<" = "<<A<endl
#define FOR(I,N) for(int I = 0; I < N; I++)

/// initializing the triangle instance counter
int Triangle::TRIANGLE_NEXT_ID = 0;

using namespace std;

void intersectFPGA(
                int i_tNumber,          /// number of triangles
                double *i_tData,        /// triangle information.    Size = i_tNumber * Triangle attribute number(12)
                int *i_tIds,            /// triangle id information. Size = i_tNumber
                int i_rNumber,          /// number of rays
                double *i_rData,        /// ray information                             Size = i_rNumber * Ray Attr number(6)
                int *o_tIds,            /// min intersection triangle ids for each ray. Size = i_rNumber
                double *o_tIntersects   /// min intersection t value for each ray       Size = i_rNumber
                )
{
    const int triangleAttributeNumber = 12; /// p1, p2, p3 and normal * 3
    const int rayAttributeNumber = 6;       /// origin and direction  * 3
    const double EPSILON = 0.0001;
    const double infinity = 10000000.0;

    double t_p1[3], t_p2[3], t_p3[3], t_normal[3];
    double r_origin[3], r_direction[3];

    int tri, ray;
    for(ray = 0; ray < i_rNumber; ray++)
    {
        int rayBase = ray * rayAttributeNumber;

        int hitAny = 0; /// for checking if a ray hit any triangle

        o_tIntersects[ray] = infinity; /// minimum t equals infinity

        /// getting ray data
        r_origin[0] = i_rData[rayBase];
        r_origin[1] = i_rData[rayBase + 1];
        r_origin[2] = i_rData[rayBase + 2];

        r_direction[0] = i_rData[rayBase + 3];
        r_direction[1] = i_rData[rayBase + 4];
        r_direction[2] = i_rData[rayBase + 5];

        for(tri = 0; tri < i_tNumber; tri++)
        {
            /// getting triangle data
            int triBase = tri * triangleAttributeNumber;

            t_p1[0] = i_tData[triBase];
            t_p1[1] = i_tData[triBase + 1];
            t_p1[2] = i_tData[triBase + 2];

            t_p2[0] = i_tData[triBase + 3];
            t_p2[1] = i_tData[triBase + 4];
            t_p2[2] = i_tData[triBase + 5];

            t_p3[0] = i_tData[triBase + 6];
            t_p3[1] = i_tData[triBase + 7];
            t_p3[2] = i_tData[triBase + 8];

            t_normal[0] = i_tData[triBase + 9];
            t_normal[1] = i_tData[triBase + 10];
            t_normal[2] = i_tData[triBase + 11];

            double a = t_p1[0] - t_p2[0], b = t_p1[0] - t_p3[0],
                    c = r_direction[0], d = t_p1[0] - r_origin[0];

            double e = t_p1[1] - t_p2[1], f = t_p1[1] - t_p3[1],
                    g = r_direction[1], h = t_p1[1] - r_origin[1];

            double i = t_p1[2] - t_p2[2], j = t_p1[2] - t_p3[2],
                    k = r_direction[2], l = t_p1[2] - r_origin[2];

            double m = f*k - g*j, n = h*k - g*l, p = f*l - h*j;
            double q = g*i - e*k, s = e*j - f*i;

            double inv_denom = 1.0/(a*m + b*q + c*s);

            double e1 = d*m - b*n - c*p;
            double beta = e1*inv_denom;

            if(beta < 0.0) continue;

            double r = e*l - h*i;
            double e2 = a*n + d*q + c*r;
            double gamma = e2 * inv_denom;

            if(gamma < 0.0)         continue;
            if(beta + gamma > 1.0)  continue;

            double e3 = a*p - b*r + d*s;
            double tmin = e3 * inv_denom;

            if(tmin < EPSILON)    continue;

            if(tmin < o_tIntersects[ray]) /// if the intersection point is the closest yet
                o_tIntersects[ray] = tmin,  /// sets the minimum t value of the ray
                o_tIds[ray] = i_tIds[tri];  /// and sets the id of the closest triangle
        }

        if(o_tIntersects[ray] == infinity) /// if no triangle was hit
        {
            /// sets output id of the ray to -1
            /// meaning that no triangle will be shaded
            o_tIds[ray] = -1;

        }
    }
}



int main(){


    Vec3d eye(0, -100, 0), lkp(0, 0, 0), up(0,0,1);
    double dist = 50, psize=1;
    int vres = 1, hres = 1;
    Camera cam = Camera(eye, lkp, up, dist, psize, vres, hres);
    Ray r = cam.getRay(0,0);
    printVec(r.origin);
    printVec(r.direction);

    /*
    Mesh m("3d_models/teddy.obj");

    for(int r = 0; r < vres; r++)
    for(int c = 0; c < hres; c++)
    {
        Ray ray = cam.getRay(r, c);
        double tMin = 10000000.0;
        int idMin = -1;
        //cout<<m.triangles.size();
        for(int i = 0; i < m.triangles.size(); i++)
        {
            double t;
            bool hit = m.triangles[i]->hit(ray, t);
            //if(hit) cout<<"t = "<<t<<endl;
            if(hit && t < tMin)
            {
                idMin = m.triangles[i]->getId();
                tMin = t;

            }
        }
    }*/

    Mesh m = Mesh();
    Triangle tri = Triangle();
    m.addTriangle(&tri);

    Scene s = Scene();
    s.addObject(&m);

    const vector<Mesh*> meshes = s.getMeshes();
    cout<<"scene time! :)\n";
    for(int i = 0; i < 1; i++)
    {
        meshes[0]->triangles[i]->printData();
    }

    printVec(r.origin); printVec(r.direction);

    Grid g1 = Grid(&s);

    g1.buildGrid();

    GridData gd = g1.exportScene();
    CameraData cd = cam.exportRays();

    double *outTData  = (double*) malloc(sizeof(double)*hres*vres);
    int *idsData = (int*) malloc(sizeof(int)*hres*vres);



    intersectFPGA(gd.idDataSize, gd.triData, gd.IdData,
                  vres*hres, cd.rayData,idsData, outTData);


    FOR(i,hres*vres) cout<<outTData[i]<< " "; cout<<endl;
    FOR(i,hres*vres) cout<<idsData[i]<< " "; cout<<endl;
    double teste;
    cout<<g1.gridVoxels[0][0]->hit(r, teste)<<endl;
    cout<<teste<<endl;

    free(outTData);
    free(idsData);
    cd.freeAll();
    gd.freeAll();

    return 0;
}
