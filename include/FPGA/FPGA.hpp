#ifdef FPGA

#ifdef _FPGA_H_
#define _FPGA_H_

#include "admxrc3.h"

##include "Application/Internal/MemoryManager.hpp"

namespace FPGA
{
	class FPGA
	{
	public:
	
		FPGA(
			unsigned int cardID,
			unsigned int memoryWindow) :
		        m_cardHandle (ADMXRC3_HANDLE_INVALID_VALUE),
		        m_cardID(cardID),
		        m_chronometer(Chrono(std::cout)),
				m_deviceWindow(memoryWindow),
		    	m_dmaChannel(0)
        { 
        	this->init();
        }

        ~FPGA(unsigned int cardID)
        {
        	this->destroy();
        }


	    Chrono m_chronometer;
	    ADMXRC3_HANDLE m_cardHandle;

	    FPGA::internal::MemoryManager m_memoryManager;

	    const static uint64_t FPGA_START = 1;
	    const static uint64_t FPGA_IDLE    = 4;
	    const static uint32_t FPGA_NO_FLAGS = 0;

	    unsigned int m_cardID;
	    unsigned int m_deviceWindow;
	    unsigned int m_dmaChannel;
	    uint64_t m_status;

	    /** Initialize FPGA information and get card handler
	    */
	    int init()
	    {
	        ADMXRC3_STATUS status = ADMXRC3_Open(m_cardID, &m_cardHandle);

	        if (status != ADMXRC3_SUCCESS) 
	        {
	            std::cerr 
	                << "Failed to open the card with index " << (unsigned long) m_cardID
	                << ": " << ADMXRC3_GetStatusString(status, TRUE) << "\n";
	            exit(EXIT_FAILURE);
	        }


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

	    int destroy()
	    {
	        ADMXRC3_STATUS status = ADMXRC3_Close(m_cardHandle);
	        if (status != ADMXRC3_SUCCESS) 
	        {
	            std::cerr 
	                << "Failed to close the card with index " << (unsigned long) m_cardID
	                << ": " << ADMXRC3_GetStatusString(status, TRUE) << "\n";
	            exit(EXIT_FAILURE);
	        }
	    }

	    
	    uint64_t getStatus()
	    {
	        this->readFPGA(
	            XINTERSECTFPGA_CONTROL_ADDR_AP_CTRL,
	            sizeof(uint64_t),
	            &m_status
	        );
	        return m_status;
	    }

	    void read(
	        size_t windowOffset, 
	        size_t length,
	        void* destinationBuffer
        ) {
	        ADMXRC3_Read(
	            m_cardHandle, 
	            m_deviceWindow, 
	            FPGA_NO_FLAGS, 
	            windowOffset, 
	            length, 
	            destinationBuffer
	        );
	    }

	    void write(
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
	    ) {
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
	                << " of card " << (unsigned long) m_cardID
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
	                << " of card " << (unsigned long) m_cardID
	                << ": " << ADMXRC3_GetStatusString(status, TRUE) << "\n";
	            exit(EXIT_FAILURE);
	        }
	    }

		/* data */
	};
}

#endif // _FPGA_H_
#endif // FPGA