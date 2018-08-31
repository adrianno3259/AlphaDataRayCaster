#ifndef _H_ACCEL_FPGA_
#define _H_ACCEL_FPGA_


#include <iostream>
#include <map>
#include <vector>
#include <memory>

namespace fpga
{

    /**
    *  Pointer type that represents a memory address inside
    *  the FPGA DDR memory. Example:
    *  fpga::ptr_t arrayBaseAddR = 0x00;
    */
    typedef uint64_t ptr_t;

    /** Struct to represent a chunk inside the FPGA DDR Memory.
    *   Used to manage
    *
    */
    typedef struct
    {
        uint64_t _start;
        unsigned long int _size;
        int _wordWidth;
        bool _free;

        inline bool isFree() const{ return _free; }

    } _MemoryChunk;


    /** Class responsible for allocating and managing the FPGA
    *   DDR memory. This class is only used by the FPGA class
    *   as a helper t organize the data
    *
    */
    class MemoryManager
    {
    private:
        std::vector<_MemoryChunk> m_memory;
    public:
        /// initializes an empty memory
        MemoryManager(const long int mem_size);
        /// do nothing
        virtual ~MemoryManager();

        /** Return the base address that could support the requested size
        * 	Throws exception if no chunk with required size is found
        * 	p.s.: remember to use std::vector::emplace and std::find_if
        *
        *   @param _size size of the memory chunk to be allocated
        *   @return an FPGA pointer that indicates the allocated address
        */
        fpga::ptr_t alloc(long int _size);

        /** Checks if an FPGA address is allocated to the user
        *   and deallocates the memory chunk by setting the chunk
        *   as 'free' in the class _MemoryChunk
        *
        *   @param _pointer FPGA pointer that indicates the allocated address
        *   @return a boolean indicating if the free operation was successful
        */
        bool free(const fpga::ptr_t& _pointer);

        void joinFreeChunks();

        void isFree(const fpga::ptr_t& _addr) const;

    };


    /**
    *   Class that implements an interface between the FPGA API and the code
    *   of the application. This class is responsible for opening the FPGA,
    *   DDR Memory management ('Allocation' and data copy), and triggering the
    *   Hardware accelerator execution.
    *
    */
    /// To save a variable (e.g. via AXI-Lite) it's necessary to have:
    ///     1. the host address of the variable containing the value that will be sent;
    ///     2. the FPGA address of where the value will be stored
    ///
    /// To save an array, on the other hand (e.g. via AXI-Full), it's required to have:
    ///     1. the host address of the beggining of the array, i.e. a pointer to an array
    ///     2. the FPGA base address of where the array will be stored
    ///     3. the FPGA address of where the FPGA base address will be stored, so the FPGA
    ///         can find it
    class FPGA
    {
    private:
        /** Handle variable. References the hardware.
        */
        // ADMXRC3_HANDLE hCard = ADMXRC3_HANDLE_INVALID_VALUE;

        std::shared_ptr<fpga::MemoryManager> m_memory;
    public:
        FPGA()
        {
            m_memory = std::make_shared<MemoryManager>(8e+9L);
            std::cout << "Objeto criado" << std::endl;
        }


        ~FPGA(){}

        /** Every accelerator input is either a value or a pointer to an array/variable.
        *   So to add a new input it's necessary to provide
        *       1. inputName: The name of the input;
        *       2. fpgaAddr: The FPGA address where the input will be read; and
        *       3. value: The value that will be transfered (a host address, i.e. pointer).
        *
        *   P.S.: It's important to note that in case of an array as input, the value will be
        *           the base FPGA address of the array, so it must be initialized with the custom
        *           malloc, memcpy and/or memset.
        *
        *   @param inputName The name of the input;
        *   @param fpgaAddr The FPGA address where the input will be read; and
        *   @param value The value that will be transfered (a host address, i.e. pointer).
        */

        /// Test if _fpgaAddr is already allocated
        template <typename T>
        void addInput(
            const std::string& _inputName,
            fpga::ptr_t _fpgaAddr,
            void* _value,
            unsigned long int _size=1
            );

        /// Searches for free space and reserve it for copy
        fpga::ptr_t fpgaMalloc(unsigned long int _size);

        /// Dealloc a memory chunk
        void fpgaFree(fpga::ptr_t _pointerAddr);

        /// copy data from in the direction defined by _direction (HostToFPGA or FPGAToHost)
        void fpgaMemcpy(fpga::ptr_t _to, void* _from, unsigned long int _size, short direction);


    };


}

#endif // _H_ACCEL_FPGA_
