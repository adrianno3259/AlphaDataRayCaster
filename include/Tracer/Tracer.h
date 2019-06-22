#ifndef _TRACER_H_
#define _TRACER_H_

#include <cstdlib>
#include <cstdio>
#include <utility>
#include <cmath>
#include <fstream>

#include "Application/Session.h"

#include "Vec3d.h"
#include "Mesh.h"
#include "Grid.h"
#include "Camera.h"
#include "Light.h"
#include "Ray.h"
#include "material/Material.h"
#include "Chrono.h"

#include "Constants.h"

#ifdef FPGA
#   include "admxrc3.h"
#	define TRUE (0==0)
#	define FALSE (0==1)
#   define XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL            0x00
#   define XINTERSECTFPGA_CONTROL_ADDR_GIE                0x04
#   define XINTERSECTFPGA_CONTROL_ADDR_IER                0x08
#   define XINTERSECTFPGA_CONTROL_ADDR_ISR                0x0c

//#   define XINTERSECTFPGA_CONTROL_ADDR_I_TNUMBER_DATA     0x10
//#   define XINTERSECTFPGA_CONTROL_BITS_I_TNUMBER_DATA     32
//#   define XINTERSECTFPGA_CONTROL_ADDR_I_TDATA_DATA       0x18
//#   define XINTERSECTFPGA_CONTROL_BITS_I_TDATA_DATA       32
//#   define XINTERSECTFPGA_CONTROL_ADDR_I_TIDS_DATA        0x20
//#   define XINTERSECTFPGA_CONTROL_BITS_I_TIDS_DATA        32
//#   define XINTERSECTFPGA_CONTROL_ADDR_I_RNUMBER_DATA     0x28
//#   define XINTERSECTFPGA_CONTROL_BITS_I_RNUMBER_DATA     32
//#   define XINTERSECTFPGA_CONTROL_ADDR_I_RDATA_DATA       0x30
//#   define XINTERSECTFPGA_CONTROL_BITS_I_RDATA_DATA       32
//#   define XINTERSECTFPGA_CONTROL_ADDR_O_TIDS_DATA        0x38
//#   define XINTERSECTFPGA_CONTROL_BITS_O_TIDS_DATA        32
//#   define XINTERSECTFPGA_CONTROL_ADDR_O_TINTERSECTS_DATA 0x40
//#   define XINTERSECTFPGA_CONTROL_BITS_O_TINTERSECTS_DATA 32

#   define XINTERSECTFPGA_CONTROL_ADDR_I_TNUMBER_DATA     0x10
#   define XINTERSECTFPGA_CONTROL_BITS_I_TNUMBER_DATA     32
#   define XINTERSECTFPGA_CONTROL_ADDR_I_TDATA_DATA       0x18
#   define XINTERSECTFPGA_CONTROL_BITS_I_TDATA_DATA       32
#   define XINTERSECTFPGA_CONTROL_ADDR_I_RNUMBER_DATA     0x20
#   define XINTERSECTFPGA_CONTROL_BITS_I_RNUMBER_DATA     32
#   define XINTERSECTFPGA_CONTROL_ADDR_I_RDATA_DATA       0x28
#   define XINTERSECTFPGA_CONTROL_BITS_I_RDATA_DATA       32
#   define XINTERSECTFPGA_CONTROL_ADDR_O_TIDS_DATA        0x30
#   define XINTERSECTFPGA_CONTROL_BITS_O_TIDS_DATA        32
#   define XINTERSECTFPGA_CONTROL_ADDR_O_TINTERSECTS_DATA 0x38
#   define XINTERSECTFPGA_CONTROL_BITS_O_TINTERSECTS_DATA 32
#endif

class Tracer{
protected:
    Application::Session& m_session;

    void saveRays(const Camera& cam)
    {
        std::fstream outFile("ray_input.in", std::fstream::out);
        int hres = cam.getHorizontalResolution();
        int vres = cam.getVerticalResolution();
        for(int r = 0; r < vres; r++)
        {
            for(int c = 0, base; c < hres; c++, base += 6)
            {
                auto&& ray = cam.getRay(r, c);
                outFile << ray.origin.x << " ";
                outFile << ray.origin.y << " ";
                outFile << ray.origin.z << " ";
                outFile << ray.direction.x << " ";
                outFile << ray.direction.y << " ";
                outFile << ray.direction.z << " ";
            }
        }
    }


    void saveTriangles(const Mesh& m)
    {
        std::fstream outFile("triangle_input.in", std::fstream::out);
        for(int i = 0, base = 0; i < m.triangles.size(); i++, base += 9)
        {
            outFile << i << " ";
            outFile << m.triangles[i]->p1.x << " ";
            outFile << m.triangles[i]->p1.y << " ";
            outFile << m.triangles[i]->p1.z << " ";
            outFile << m.triangles[i]->p2.x << " ";
            outFile << m.triangles[i]->p2.y << " ";
            outFile << m.triangles[i]->p2.z << " ";
            outFile << m.triangles[i]->p3.x << " ";
            outFile << m.triangles[i]->p3.y << " ";
            outFile << m.triangles[i]->p3.z << "\n";
        }
    }


public:     
    Tracer(Application::Session& sess) :
        m_session(sess){}

    virtual void render(int frame) = 0;
};


class CPUTracer : public Tracer{
public:     
    CPUTracer(Application::Session& sess) :
        Tracer(sess) {}

    virtual void render(int frame) 
    {

        if(m_session.meshes.size() > 0 && m_session.materials.size() > 0)
        {
            std::cout << "LOG: Beginning to process in the CPU" << std::endl;
            std::cout 
                << "LOG: Image of size "
                << m_session.camera->getHorizontalResolution() 
                << "x" 
                << m_session.camera->getVerticalResolution() 
                << std::endl;

            //saveTriangles(m_session.meshes[0]);
            //saveRays(*(m_session.camera));
            //std::fstream outFile("intersections.out", std::fstream::out);
            int r, c;
            for(r = 0; r < m_session.camera->getVerticalResolution(); r++)
            for(c = 0; c < m_session.camera->getHorizontalResolution(); c++)
            {
                Color L;
                Ray ray = m_session.camera->getRay(r, c);
    
                Intersection it;
    
                double tMin = 1e9;
                int idMin = -1, iMin = -1;
                for(int i = 0; i < m_session.meshes[0].triangles.size(); i++)
                {
                    double t;
                    bool hit = m_session.meshes[0].triangles[i]->hit(ray, t);
                    if(hit && t < tMin)
                    {
                        idMin = m_session.meshes[0].triangles[i]->getId();
                        tMin = t;
                        iMin = i;

                        it.t = t;
                        it.triangleId = idMin;
    
                    }
                }
                
                //outFile << idMin << " " << tMin << "\n";

                if(idMin != -1)
                {
    
                    it.normalVector = m_session.meshes[0].triangles[iMin]->normal;
                    it.hit = true;
                    it.rayDirection = ray.direction;
                    it.hitPoint = ray.rayPoint(it.t);
                    it.triangleId = iMin;
    
                    L = m_session.materials[0]->shade(it, m_session.lights);
    
                }
                m_session.frames[0].setPixel(c, r, L.clamp());
            }
        }
    }
};

#ifdef FPGA

class FPGATracer : public Tracer
{

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


    void prepareTriangles(double *tData, int *idData, const Mesh& m)
    {
        for(int i = 0; i < m.triangles.size(); i++)
        {
            idData[i] = i;
            int base = i * 9;
            tData[base + 0] = m.triangles[i]->p1.x;
            tData[base + 1] = m.triangles[i]->p1.y;
            tData[base + 2] = m.triangles[i]->p1.z;
            tData[base + 3] = m.triangles[i]->p2.x;
            tData[base + 4] = m.triangles[i]->p2.y;
            tData[base + 5] = m.triangles[i]->p2.z;
            tData[base + 6] = m.triangles[i]->p3.x;
            tData[base + 7] = m.triangles[i]->p3.y;
            tData[base + 8] = m.triangles[i]->p3.z;
        }
    }

public:
    FPGATracer(Application::Session& sess) :
        Tracer(sess),
        m_cardHandle (ADMXRC3_HANDLE_INVALID_VALUE),
        m_chronometer(Chrono(std::cout))
        {
            addr_tris = 0;
            addr_ids = sizeof(double) * FPGA_MAX_TRIS * FPGA_TRI_ATTR_NUMBER;
            addr_rays = addr_ids + (sizeof(int) * FPGA_MAX_TRIS);
            addr_outInter = addr_rays + (sizeof(double) * FPGA_MAX_RAYS * FPGA_RAY_ATTR_NUMBER);
            addr_outIds = addr_outInter + (sizeof(double) * FPGA_MAX_RAYS);

        }

    Chrono m_chronometer;
    const unsigned int FPGA_CARD_ID = 0;
    ADMXRC3_HANDLE m_cardHandle;
    int m_numberOfRays;
    int m_numberOfTriangle;


    /** Initialize FPGA information and get card handler
    */
    int initFPGA()
    {
        ADMXRC3_STATUS status = ADMXRC3_Open(FPGA_CARD_ID, &m_cardHandle);

        if (status != ADMXRC3_SUCCESS) 
        {
            std::cerr 
                << "Failed to open the card with index " << (unsigned long) FPGA_CARD_ID
                << ": " << ADMXRC3_GetStatusString(status, TRUE) << "\n";
            exit(EXIT_FAILURE);
        }

        //auto fpgaInfo = ADMXRC3_GetFPGAInfo;
        //std::cout << "[info] : Opening FPGA: " 
        //        << std::string(fpgaInfo.Identifier) << ":\n"
        //        << "\tDevice Code: " << fpgaInfo.DeviceCode << "\n"
        //        << "\tFamily Code: " << fpgaInfo.FamilyInfo << "\n"
        //        << "\n";


        for(int i = 0; i < 3; i++)
        {

            ADMXRC3_WINDOW_INFO windowInfo;
            status = ADMXRC3_GetWindowInfo(
                m_cardHandle,
                i,
                //m_deviceWindow,
                &windowInfo
            );

            std::cout << "[info] : Memory window " << i << " information:\n"
                << "\tBus Size: " << windowInfo.BusSize << " B\n"
                << "\tBus Base: " << windowInfo.BusBase << " B\n"
                << "\tLocal Size: " << windowInfo.LocalSize << " B\n"
                << "\tLocal Base: " << windowInfo.LocalBase << " B\n"
                << "\tVirtual Size: " << windowInfo.VirtualSize << " B\n"
                << "\n";
        }
    }

    int destroyFPGA()
    {
        ADMXRC3_STATUS status = ADMXRC3_Close(m_cardHandle);
        if (status != ADMXRC3_SUCCESS) 
        {
            std::cerr 
                << "Failed to close the card with index " << (unsigned long) FPGA_CARD_ID
                << ": " << ADMXRC3_GetStatusString(status, TRUE) << "\n";
            exit(EXIT_FAILURE);
        }
    }


    const uint64_t FPGA_RUNNING = 1;
    const uint64_t FPGA_IDLE    = 4;
    const uint32_t FPGA_NO_FLAGS = 0;
    
    unsigned int m_deviceWindow = 0;
    unsigned int m_dmaChannel = 0;
    uint64_t m_fpgaStatus;

    void _updateStatusFPGA()
    {
        
        this->readFPGA(
            XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL,
            sizeof(uint64_t),
            &m_fpgaStatus
        );
    }

    void readFPGA(
        size_t windowOffset, 
        size_t length,
        void* destinationBuffer)
    {
        ADMXRC3_Read(
            m_cardHandle, 
            m_deviceWindow, 
            FPGA_NO_FLAGS, 
            windowOffset, 
            length, 
            destinationBuffer
        );
    }

    void writeFPGA(
        size_t windowOffset, 
        size_t length,
        void* destinationBuffer
    ) {
        ADMXRC3_Write(
            m_cardHandle, 
            m_deviceWindow, 
            FPGA_NO_FLAGS, 
            windowOffset, 
            length, 
            destinationBuffer);
    }

    void writeDMA(
        void* addrFromCPU,
        size_t dataLength,
        uint64_t addrToFPGA
        )
    {
        ADMXRC3_STATUS status = 
            ADMXRC3_WriteDMA(
                m_cardHandle, 
                m_dmaChannel, 
                FPGA_NO_FLAGS, 
                addrFromCPU, 
                dataLength, 
                addrToFPGA
            );

        if (status != ADMXRC3_SUCCESS) 
        {
            std::cerr 
                << "Failed to write to the DMA channel " << m_dmaChannel 
                << " of card " << (unsigned long) FPGA_CARD_ID
                << ": " << ADMXRC3_GetStatusString(status, TRUE) << "\n";
            exit(EXIT_FAILURE);
        }
    }

    void readDMA(
        void* addrToCPU,
        size_t dataLength,
        uint64_t addrFromFPGA
    ) {

        ADMXRC3_STATUS status = 
            ADMXRC3_ReadDMA(
                m_cardHandle, 
                m_dmaChannel, 
                FPGA_NO_FLAGS, 
                addrToCPU, 
                dataLength, 
                addrFromFPGA
            );

        if (status != ADMXRC3_SUCCESS) 
        {
            std::cerr 
                << "Failed to read the DMA channel " << m_dmaChannel 
                << " of card " << (unsigned long) FPGA_CARD_ID
                << ": " << ADMXRC3_GetStatusString(status, TRUE) << "\n";
            exit(EXIT_FAILURE);
        }
    }

    const int FPGA_TRI_ATTR_NUMBER = 9;
    const int FPGA_RAY_ATTR_NUMBER = 6;

    const int FPGA_MAX_TRIS = 50000;
    const int FPGA_MAX_RAYS = 921600;

    /// Declaring the base addresses of the arrays in the
    /// FPGA DDR memory
    uint64_t addr_tris;
    uint64_t addr_ids;
    uint64_t addr_rays;
    uint64_t addr_outInter;
    uint64_t addr_outIds;

    std::vector<int> offloadGeometry(const Mesh& mesh)
    {
        int nTris = mesh.triangles.size();

        double* tData = new double[nTris * FPGA_TRI_ATTR_NUMBER];
        std::vector<int> idData(nTris);
        
        prepareTriangles(tData, static_cast<int*>(&(idData[0])), mesh);

        /// Writing tData
        m_chronometer.start();
        this->writeDMA(tData, nTris * FPGA_TRI_ATTR_NUMBER * sizeof(double), addr_tris);
        m_chronometer.stop("Write triangles to DMA");


        delete[] tData;
        return idData;
    }

    virtual void render(int frame) {

        /// Opening FPGA card
        this->initFPGA();
        

        int hres = m_session.camera->getHorizontalResolution();
        int vres = m_session.camera->getVerticalResolution();
        m_numberOfRays = vres * hres;

        /// Initializing the basic information of the raytracing session
        int nRays = vres * hres, 
            nTris = m_session.meshes[0].triangles.size();

        // std::vector containing the global identifier of the
        // triangles sent to the accelerator
        auto triangleIdMap = this->offloadGeometry(m_session.meshes[0]);

        uint64_t NUM_RAYS = nRays;
        uint64_t RAY_SIZE = 6;
        uint64_t MAX_RAYS = 921600;

        uint64_t NUM_TRIS = nTris;
        uint64_t TRI_SIZE = 9;
        uint64_t MAX_TRIS = 50000;
    
        double *rData = new double[nRays * Ray::NUM_ATTRIBUTES];
        int *outIds = new int[nRays];
        double *outInter = new double[nRays];
    
        /// preparing data structures to send to the FPGA
        prepareRays(rData, *m_session.camera);
        
        /// Writing the new command (idle)
        m_fpgaStatus = 4;
        this->writeFPGA(
            XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, 
            sizeof(uint64_t), 
            &m_fpgaStatus);

        /// Getting the status of the Accelerator by reading the CTRL address
        //this->_updateStatusFPGA();
        std::cout << "[info] : FPGA status = "<<  m_fpgaStatus << "\n";


        /// Writing to the input addresses
        /// Number of Triangles
        this->writeFPGA(
            XINTERSECTFPGA_CONTROL_ADDR_I_TNUMBER_DATA,
            sizeof(uint64_t), 
            &nTris);

        /// tData base address
        this->writeFPGA(
            XINTERSECTFPGA_CONTROL_ADDR_I_TDATA_DATA, 
            sizeof(uint64_t), 
            &addr_tris);

        /// Number of Rays
        this->writeFPGA(
            XINTERSECTFPGA_CONTROL_ADDR_I_RNUMBER_DATA, 
            sizeof(uint64_t), 
            &nRays);

        /// rData base address
        this->writeFPGA(
            XINTERSECTFPGA_CONTROL_ADDR_I_RDATA_DATA, 
            sizeof(uint64_t), 
            &addr_rays);

        /// outIds base address - triangle id output
        this->writeFPGA(
            XINTERSECTFPGA_CONTROL_ADDR_O_TIDS_DATA, 
            sizeof(uint64_t), 
            &addr_outIds);

        /// outInter base address - triangle intersection distance output
        this->writeFPGA(
            XINTERSECTFPGA_CONTROL_ADDR_O_TINTERSECTS_DATA, 
            sizeof(uint64_t), 
            &addr_outInter);


        /// FPGA Memory copy of the input arrays via DMA
        ///---------------------------------------------------
        ///---------------------dma writes--------------------
        ///---------------------------------------------------

        /// Writing rData
        m_chronometer.start();
        this->writeDMA(rData, NUM_RAYS * RAY_SIZE * sizeof(double), addr_rays);
        m_chronometer.stop("Writing Rays to DMA");
        std::cout << "[info] : Finished writing input to DMA\n";

        this->_updateStatusFPGA();

        if(m_fpgaStatus == FPGA_IDLE)
        {
            std::cout << "core is idle, m_fpgaStatus = " << m_fpgaStatus << "\n";
        }

        /// Write control value = 1 (start)
        m_fpgaStatus = FPGA_RUNNING;
        
        this->writeFPGA(
            XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, 
            sizeof(uint64_t), 
            &m_fpgaStatus
        );

        /// Polling the FPGA for the results
        do {
            this->_updateStatusFPGA();
        } while(m_fpgaStatus != FPGA_IDLE);

        std::cout << "[info] : done!\n";


        /// Getting the result back from the FPGA

        /// Id of the triangles hit by the sent rays
        this->readDMA(outIds, NUM_RAYS * sizeof(int), addr_outIds);
        
        /// Intersection parameter of where the triangle was hit
        this->readDMA(outInter, NUM_RAYS * sizeof(double), addr_outInter);


        // Shading the results
        for(int r = 0; r < vres; r++)
        for(int c = 0; c < hres; c++)
        {
            Color L;
            /// Identifier of the ray that was sent to the FPGA
            int rayId = r * hres + c;
            /// Identifier of the triangle hit by the current ray
            int hitId = outIds[rayId];
            bool hit = (hitId != -1);

            Intersection it;

            Ray ray = m_session.camera->getRay(r, c);
            
            if (hit) {
                hitId = triangleIdMap[hitId];

                Vec3d wo =
                    Vec3d(
                        rData[rayId * Ray::NUM_ATTRIBUTES + 3],
                        rData[rayId * Ray::NUM_ATTRIBUTES + 4],
                        rData[rayId * Ray::NUM_ATTRIBUTES + 5]
                    );// -ray.direction;

                //double dot = m->triangles[hitId]->normal * wo;
                //Color lightInfluence = light->mColor * light->mIntensity;
                //L = (matCol * matDif * INV_PI) * lightInfluence * dot;

                it.normalVector = m_session.meshes[0].triangles[hitId]->normal;
                it.hit = true;
                it.rayDirection = wo;
                it.t = outInter[rayId];
                it.hitPoint = ray.rayPoint(it.t);
                it.triangleId = hitId;

                L = m_session.materials[0]->shade(it, m_session.lights);
            }

            m_session.frames[0].setPixel(c, r, L);

        }
      

        
        this->destroyFPGA();

        delete[] rData;
        delete[] outIds;
        delete[] outInter;
    }

};
#endif
	
#endif