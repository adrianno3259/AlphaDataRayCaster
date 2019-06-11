#ifndef _XINTERSECTFPGA_H_
#define _XINTERSECTFPGA_H_

#include "FPGA/FPGA.hpp"


#include "Chrono.h"

class XIntersectFPGA
{
    const size_t CONTROL_ADDR_AP_CTRL            = 0x00;
    const size_t CONTROL_ADDR_GIE                = 0x04;
    const size_t CONTROL_ADDR_IER                = 0x08;
    const size_t CONTROL_ADDR_ISR                = 0x0c;
    const size_t CONTROL_ADDR_I_TNUMBER_DATA     = 0x10;
    const size_t CONTROL_BITS_I_TNUMBER_DATA     = 32;
    const size_t CONTROL_ADDR_I_TDATA_DATA       = 0x18;
    const size_t CONTROL_BITS_I_TDATA_DATA       = 32;
    const size_t CONTROL_ADDR_I_TIDS_DATA        = 0x20;
    const size_t CONTROL_BITS_I_TIDS_DATA        = 32;
    const size_t CONTROL_ADDR_I_RNUMBER_DATA     = 0x28;
    const size_t CONTROL_BITS_I_RNUMBER_DATA     = 32;
    const size_t CONTROL_ADDR_I_RDATA_DATA       = 0x30;
    const size_t CONTROL_BITS_I_RDATA_DATA       = 32;
    const size_t CONTROL_ADDR_O_TIDS_DATA        = 0x38;
    const size_t CONTROL_BITS_O_TIDS_DATA        = 32;
    const size_t CONTROL_ADDR_O_TINTERSECTS_DATA = 0x40;
    const size_t CONTROL_BITS_O_TINTERSECTS_DATA = 32; 

    FPGA::FPGA& m_fpga;

    FPGA::fpgaPtr_t m_raysAddr;
    FPGA::fpgaPtr_t m_triangleIdsAddr;
    FPGA::fpgaPtr_t m_trianglesAddr;

    uint64_t m_numberOfTriangles;
    uint64_t m_numberOfRays;

    FPGA::fpgaPtr_t m_outIntersectsAddr;
    FPGA::fpgaPtr_t m_outIdsAddr;
    
    Chrono m_chronometer;

public:
    XIntersectFPGA(FPGA::FPGA& fpga) :
        m_fpga(fpga),
        m_chronometer(Chrono(std::cout))
    {

    }

    void start()
    {
        uint64_t startCode = 1;
        /// Writing the new command (idle)
        m_fpga.write(
            XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, 
            sizeof(uint64_t), 
            &startCode);
    }

    bool isRunning()
    {
    	uint64_t status;
        /// Writing the new command (idle)
        m_fpga.read(
            XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL, 
            sizeof(uint64_t), 
            &status);
        return status == 1;
    }

    void setTriangleNumber(uint64_t nTris)
    {
        m_numberOfTriangles = nTris;
        /// Writing to the input addresses
        /// Number of Triangles
        m_fpga.write(
            CONTROL_ADDR_I_TNUMBER_DATA,
            sizeof(uint64_t), 
            &nTris);
    }

    void setTriangleIdsAddr(FPGA::fpgaPtr_t tidsAddr)
    {
    	m_triangleIdsAddr = tidsAddr;
        /// idData base address
        m_fpga.write(
            XINTERSECTFPGA_CONTROL_ADDR_I_TIDS_DATA, 
            sizeof(uint64_t), 
            &tidsAddr);
    }

    void setTrianglesAddr(FPGA::fpgaPtr_t trianglesAddr)
    {
    	m_trianglesAddr = trianglesAddr;
        /// tData base address
        m_fpga.write(
            XINTERSECTFPGA_CONTROL_ADDR_I_TDATA_DATA, 
            sizeof(uint64_t), 
            &trianglesAddr);
    }

    void setRayNumber(uint64_t nRays)
    {
        m_numberOfRays = nRays;
        /// Number of Rays
        m_fpga.write(
            XINTERSECTFPGA_CONTROL_ADDR_I_RNUMBER_DATA, 
            sizeof(uint64_t), 
            &nRays);
    }

    void setRaysAddr(FPGA::fpgaPtr_t raysAddr)
    {
    	m_raysAddr = raysAddr;
        /// rData base address
        m_fpga.write(
            XINTERSECTFPGA_CONTROL_ADDR_I_RDATA_DATA, 
            sizeof(uint64_t), 
            &raysAddr);
    }

    void setOutputIdsAddr(FPGA::fpgaPtr_t outIdsAddr)
    {
        m_outIdsAddr = outIdsAddr;
        /// outIds base address - triangle id output
        m_fpga.write(
            XINTERSECTFPGA_CONTROL_ADDR_O_TIDS_DATA, 
            sizeof(uint64_t), 
            &outIdsAddr);
    }

    void setOutputIntersectsAddr(FPGA::fpgaPtr_t outInterAddr){
        m_outIntersectsAddr = outInterAddr;
        /// outInter base address - triangle intersection distance output
        m_fpga.write(
            XINTERSECTFPGA_CONTROL_ADDR_O_TINTERSECTS_DATA, 
            sizeof(uint64_t), 
            &outInterAddr);   
    }


    void offloadPrimaryRays(const Camera& cam)
    {
    	int hres = cam.getHorizontalResolution();
        int vres = cam.getVerticalResolution();
    	int nRays = vres * hres;

    	double* rData = new double[nRays * Ray::NUM_ATTRIBUTES];

    	for(int r = 0; r < vres; r++)
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

		m_chronometer.start();
        m_fpga.writeDMA(
        	rData, 
        	nRays * Ray::NUM_ATTRIBUTES * sizeof(double), 
        	m_raysAddr
    	);
        m_chronometer.stop("Write triangles to DMA");        

        delete[] rData;
    }


    void offloadGeometry(const Mesh& mesh)
    {
        int nTris = mesh.triangles.size();

        double* tData = new double[nTris * Triangle::NUM_ATTRIBUTES];
        int* idData = new int[nTris];
        
        for(int i = 0; i < mesh.triangles.size(); i++)
        {
            idData[i] = i;
            
            int base = i * 9;
            tData[base + 0] = mesh.triangles[i]->p1.x;
            tData[base + 1] = mesh.triangles[i]->p1.y;
            tData[base + 2] = mesh.triangles[i]->p1.z;
            tData[base + 3] = mesh.triangles[i]->p2.x;
            tData[base + 4] = mesh.triangles[i]->p2.y;
            tData[base + 5] = mesh.triangles[i]->p2.z;
            tData[base + 6] = mesh.triangles[i]->p3.x;
            tData[base + 7] = mesh.triangles[i]->p3.y;
            tData[base + 8] = mesh.triangles[i]->p3.z;
        }

        /// Writing tData
        m_chronometer.start();
        m_fpga.writeDMA(
        	tData, 
        	nTris * Triangle::NUM_ATTRIBUTES * sizeof(double), 
        	m_trianglesAddr);
        m_chronometer.stop("Write triangles to DMA");
        
        /// Writing idData
        m_chronometer.start();
        m_fpga.writeDMA(
        	idData, 
        	nTris * sizeof(int), 
        	m_triangleIdsAddr);
        m_chronometer.stop("Write triangles IDs to DMA");

        delete[] tData;
        delete[] idData;
    }

    void getResults(
        std::vector<int>& outIds, 
        std::vector<double>& outIntersects
    ) {

        std::cout << "[info] : Obtaining results for " << m_numberOfRays << " rays.";
        outIds.resize(m_numberOfRays);
        outIntersects.resize(m_numberOfRays);

        // Ids of the intersected triangles
        m_fpga.readDMA(
            &(outIds[0]), 
            m_numberOfRays * sizeof(int), 
            m_outIdsAddr);
        
        /// Intersection parameter of where the triangle was hit for each ray
        m_fpga.readDMA(
            &(outIntersects[0]), 
            m_numberOfRays * sizeof(double), 
            m_outIntersectsAddr);
    }

};


#endif