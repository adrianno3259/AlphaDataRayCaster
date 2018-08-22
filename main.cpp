#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <utility>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>
#include <fstream>

#include "include/Vec3d.h"
#include "include/Mesh.h"
#include "include/GridData.h"
#include "include/Triangle.h"
#include "include/Grid.h"
#include "include/Camera.h"
#include "include/CameraData.h"
#include "include/Image.h"
#include "include/Light.h"
//#include "include/TriangleBase.h"

#define FPGA

#ifdef FPGA
#   include "admxrc3.h"
#endif

#define PV(A) cout<<#A<<" = "<<A<endl
#define FOR(I,N) for(int I = 0; I < N; I++)

#define INV_PI 0.318309886

#define TRUE (0==0)
#define FALSE (0==1)

#define XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL            0x00
#define XINTERSECTFPGA_CONTROL_ADDR_GIE                0x04
#define XINTERSECTFPGA_CONTROL_ADDR_IER                0x08
#define XINTERSECTFPGA_CONTROL_ADDR_ISR                0x0c
#define XINTERSECTFPGA_CONTROL_ADDR_I_TNUMBER_DATA     0x10
#define XINTERSECTFPGA_CONTROL_BITS_I_TNUMBER_DATA     32
#define XINTERSECTFPGA_CONTROL_ADDR_I_TDATA_DATA       0x18
#define XINTERSECTFPGA_CONTROL_BITS_I_TDATA_DATA       32
#define XINTERSECTFPGA_CONTROL_ADDR_I_TIDS_DATA        0x20
#define XINTERSECTFPGA_CONTROL_BITS_I_TIDS_DATA        32
#define XINTERSECTFPGA_CONTROL_ADDR_I_RNUMBER_DATA     0x28
#define XINTERSECTFPGA_CONTROL_BITS_I_RNUMBER_DATA     32
#define XINTERSECTFPGA_CONTROL_ADDR_I_RDATA_DATA       0x30
#define XINTERSECTFPGA_CONTROL_BITS_I_RDATA_DATA       32
#define XINTERSECTFPGA_CONTROL_ADDR_O_TIDS_DATA        0x38
#define XINTERSECTFPGA_CONTROL_BITS_O_TIDS_DATA        32
#define XINTERSECTFPGA_CONTROL_ADDR_O_TINTERSECTS_DATA 0x40
#define XINTERSECTFPGA_CONTROL_BITS_O_TINTERSECTS_DATA 32

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


void prepareRays(double* rData, const Camera& cam)
{
    int hres = cam.getHorizontalResolution();
    int vres = cam.getVerticalResolution();
    for(int r = 0; r < vres; r++)
    {
        for(int c = 0; c < hres; c++)
        {
            int base = ((r * hres) + c) * 6;
            auto&& ray = cam.getRay(r, c);
            rData[base + 0] = ray.origin.x;
            rData[base + 1] = ray.origin.y;
            rData[base + 2] = ray.origin.z;
            rData[base + 3] = ray.direction.x;
            rData[base + 4] = ray.direction.y;
            rData[base + 5] = ray.direction.z;
        }
    }

}


void prepareTriangles(double *tData, int *idData, const std::shared_ptr<Mesh>& m)
{
    for(int i = 0; i < m->triangles.size(); i++)
    {
        idData[i] = i;
        int base = i * 9;
        tData[base + 0] = m->triangles[i]->p1.x;
        tData[base + 1] = m->triangles[i]->p1.y;
        tData[base + 2] = m->triangles[i]->p1.z;
        tData[base + 3] = m->triangles[i]->p2.x;
        tData[base + 4] = m->triangles[i]->p2.y;
        tData[base + 5] = m->triangles[i]->p2.z;
        tData[base + 6] = m->triangles[i]->p3.x;
        tData[base + 7] = m->triangles[i]->p3.y;
        tData[base + 8] = m->triangles[i]->p3.z;
    }
}



class FPGA_Tracer{

private:

    int m_numberOfRays;
    int m_numberOfTriangles;


    unsigned long m_index;
    ADMXRC3_STATUS m_status;
    ADMXRC3_HANDLE m_hCard;

    const int FPGA_TRI_ATTR_NUMBER = 9;
    const int FPGA_RAY_ATTR_NUMBER = 6;
    const int FPGA_MAX_TRIS = 50000;
    const int FPGA_MAX_RAYS = 40000;


    /** This array is used to receive the output data from the FPGA.
    *   It stores the ids of the closest triangle to the ray of number
    *   i = 0, ..., nRays
    */
    int *m_outputIds;

    /** Used to receive the output data from the FPGA.
    *   It stores the intersection parameter t of the closest triangle to the 
    *   ray of number i = 0, ..., nRays. 
    */
    double *m_outputIntersects;

public:

    FPGA_Tracer() :
        m_index(0),
        m_hCard(ADMXRC3_HANDLE_INVALID_VALUE)
    {
        uint64_t NUM_RAYS = 40000;
        uint64_t RAY_SIZE = 6;
        uint64_t MAX_RAYS = 40000;

        uint64_t NUM_TRIS = 2000;
        uint64_t TRI_SIZE = 9;
        uint64_t MAX_TRIS = 50000;

    }

    /*
    void prepareRays(const Camera& cam)
    {
        int hres = cam.getHorizontalResolution();
        int vres = cam.getVerticalResolution();
        for(int r = 0; r < vres; r++)
        {
            for(int c = 0; c < hres; c++)
            {
                int base = ((r * hres) + c) * 6;
                auto&& ray = cam.getRay(r, c);
                rData[base + 0] = ray.origin.x;
                rData[base + 1] = ray.origin.y;
                rData[base + 2] = ray.origin.z;
                rData[base + 3] = ray.direction.x;
                rData[base + 4] = ray.direction.y;
                rData[base + 5] = ray.direction.z;
            }
        }

    }


    void prepareTriangles(const std::shared_ptr<Mesh>& m)
    {
        for(int i = 0; i < m->triangles.size(); i++)
        {
            idData[i] = i;
            int base = i * 9;
            tData[base + 0] = m->triangles[i]->p1.x;
            tData[base + 1] = m->triangles[i]->p1.y;
            tData[base + 2] = m->triangles[i]->p1.z;
            tData[base + 3] = m->triangles[i]->p2.x;
            tData[base + 4] = m->triangles[i]->p2.y;
            tData[base + 5] = m->triangles[i]->p2.z;
            tData[base + 6] = m->triangles[i]->p3.x;
            tData[base + 7] = m->triangles[i]->p3.y;
            tData[base + 8] = m->triangles[i]->p3.z;
        }
    }

    */
};


#define DEBUG

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



    double hostIntersects[hres * vres];
    int hostIds[hres * vres];

    
    #ifndef FPGA
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
    #else

    /// Initializing the basic information of the raytracing session
    int nRays = vres * hres, nTris = m->triangles.size();
    const int FPGA_TRI_ATTR_NUMBER = 9;

    const int FPGA_MAX_TRIS = 50000;
    const int FPGA_MAX_RAYS = 40000;

    unsigned long index = 0;
    ADMXRC3_STATUS status;
    ADMXRC3_HANDLE hCard = ADMXRC3_HANDLE_INVALID_VALUE;

	uint64_t NUM_RAYS = 40000;
	uint64_t RAY_SIZE = 6;
	uint64_t MAX_RAYS = 40000;

	uint64_t NUM_TRIS = 2000;
	uint64_t TRI_SIZE = 9;
	uint64_t MAX_TRIS = 50000;

    double *rData, *tData, *outInter;
    int *idData, *outIds;

    /// allocating the memory needed
    try
    {
        rData = new double[nRays * Ray::NUM_ATTRIBUTES];
        tData = new double[nTris * FPGA_TRI_ATTR_NUMBER];
        idData = new int[nTris];
        outIds = new int[nRays];
        outInter = new double[nRays];
    }
    catch(const std::bad_alloc& al)
    {
        cerr << "It wasn't possible to alloc a memory of this size...\nMessage: ";
        cerr << al.what() << "\n";
        exit(EXIT_FAILURE);
    }

    /// preparing data structures to send to the FPGA
    prepareRays(rData, cam);
    prepareTriangles(tData, idData, m);

    /// Declaring the base addresses of the arrays in the
    /// FPGA DDR memory
    uint64_t addr_tris = 0;
	uint64_t addr_ids = sizeof(double) * FPGA_MAX_TRIS * FPGA_MAX_TRIS;
	uint64_t addr_rays = addr_ids + (sizeof(int) * FPGA_MAX_TRIS);
	uint64_t addr_outInter = addr_rays + (sizeof(double) * FPGA_MAX_RAYS * Ray::NUM_ATTRIBUTES);
	uint64_t addr_outIds = addr_outInter + (sizeof(double) * FPGA_MAX_RAYS);
    uint64_t ctrl = 0;

	/// Opening FPGA card
	status = ADMXRC3_Open(0, &hCard);
	if (status != ADMXRC3_SUCCESS) {
        fprintf(
            stderr,
            "Failed to open card with index %lu: %s\n",
            (unsigned long) index,
            ADMXRC3_GetStatusString(status, TRUE)
        );
		return -1;
	}

    /// Getting the status of the Accelerator by reading the CTRL address
    printf("IP Status:\n");
	ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);
	printf("status 0x%lx\n",ctrl);

    /// Writing the new command (idle)
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);

	/// Writing to the input addresses
	/// Number of Triangles
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_TNUMBER_DATA, sizeof(uint64_t), &nTris);
	/// idData base address
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_TIDS_DATA, sizeof(uint64_t), &addr_ids);
	/// tData base address
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_TDATA_DATA, sizeof(uint64_t), &addr_tris);
    /// Number of Rays
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_RNUMBER_DATA, sizeof(uint64_t), &nRays);
	/// rData base address
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_I_RDATA_DATA, sizeof(uint64_t), &addr_rays);
    /// outIds base address - triangle id output
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_O_TIDS_DATA, sizeof(uint64_t), &addr_outIds);
	/// outInter base address - triangle intersection distance output
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_O_TINTERSECTS_DATA, sizeof(uint64_t), &addr_outInter);


	/// FPGA Memory copy of the input arrays via DMA
    ///---------------------------------------------------
	///---------------------dma writes--------------------
	///---------------------------------------------------

    /// Writing tData
	status = ADMXRC3_WriteDMA(hCard, 0, 0, tData, nTris * FPGA_TRI_ATTR_NUMBER * sizeof(double), addr_tris);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"Triangle data write DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}

    /// Writing idData
	status = ADMXRC3_WriteDMA(hCard, 0, 0, idData, NUM_TRIS * sizeof(int), addr_ids);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"Id data write DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}

    /// Writing rData
	status = ADMXRC3_WriteDMA(hCard, 0, 0, rData, NUM_RAYS * RAY_SIZE * sizeof(double), addr_rays);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"Triangle data write DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}
    printf("write dma done!\n");


    ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);

    if(ctrl == 4)
    {
            printf("core is idle, ctrl = %ld\n",ctrl);
    }

    ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);

    if(ctrl == 4)
    {
        cout << "core is idle, ctrl = " << ctrl << "\n";
    }

    /// Write control value = 1 (start)
    ctrl = 1;
	ADMXRC3_Write(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);

	/// Polling the FPGA for the results
	ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);
	while(ctrl != 4)
	{
		ADMXRC3_Read(hCard, 0, 0, XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, sizeof(uint64_t), &ctrl);
	}
	printf("done!\n");


	/// Getting the result back from the FPGA

	/// Id of the triangles hit by the sent rays
	status = ADMXRC3_ReadDMA(hCard, 0, 0, outIds, NUM_RAYS * sizeof(int), addr_outIds);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"outIds read DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}

    /// Intersection parameter of where the triangle was hit
	status = ADMXRC3_ReadDMA(hCard, 0, 0, outInter, NUM_RAYS * sizeof(double), addr_outInter);
	if (status != ADMXRC3_SUCCESS) {
		fprintf(stderr,"outInter read DMA transfer failed: %s\n", ADMXRC3_GetStatusString(status, TRUE));
		return -1;
	}


    int r, c;
    for(r = 0; r < vres; r++)
    for(c = 0; c < hres; c++)
    {
        Color L;
        /// Identifier of the ray that was sent to the FPGA
        int rayId = r * hres + c;
        /// Identifier of the triangle hit by the current ray
        int hitId = outIds[rayId];

        if(hitId != -1)
        {
            Vec3d wo =
                Vec3d(
                    -rData[rayId * Ray::NUM_ATTRIBUTES + 3],
                    -rData[rayId * Ray::NUM_ATTRIBUTES + 4],
                    -rData[rayId * Ray::NUM_ATTRIBUTES + 5]
                );// -ray.direction;

            double dot = m->triangles[hitId]->normal * wo;
            Color lightInfluence = light->mColor * light->mIntensity;
            L = (matCol * matDif * INV_PI) * lightInfluence * dot;
        }

        im->setPixel(c, r, L);
    }

    delete[] rData;
    delete[] tData;
    delete[] idData;
    delete[] outIds;
    delete[] outInter;

    #endif


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
