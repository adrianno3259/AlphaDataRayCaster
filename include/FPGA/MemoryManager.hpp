#ifdef FPGA
#ifdef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_


#include <vector>

namespace FPGA{

	typedef uint64_t fpgaPtr_t;

	namespace internal {

		/** Class that models a memory manager
		*	for the Card ADM-PCIE-KU3 by Alpha-Data
		*
		*/
		class MemoryManager
		{
			/** Stores information about a single
			*	and continuous memory block
			*/
			struct _MemoryChunk
			{
				uint64_t baseAddr;
				unsigned int length;
				bool isFree;
			};

			std::vector<_MemoryChunk> m_memoryBlocks;

			const int NUM_BASE_MEMORY_BLOCKS = 10;

		public:
			MemoryManager(unsigned int totalMemorySize)
			{
				m_memoryBlocks.reserve(NUM_BASE_MEMORY_BLOCKS);
				m_memoryBlocks.push_back({
					0, // Base Address 0
					totalMemorySize, // Initial available memory
					true // memory starts free
				});
			}
			
			~MemoryManager()
			{

			}

			fpgaPtr_t alloc(unsigned int length)
			{
				auto begin = m_memoryBlocks.begin();
				auto end = m_memoryBlocks.end();
				for(auto blockIt = begin; blockIt != end; blockIt++)
				{
					auto& block = *blockIt;
					if(block.isFree)
					{
						if(block.length == length)
						{
							block.isFree = false;
							return block.m_baseAddr;
						}
						else if(block.length > length)
						{
							_MemoryChunk newChunk = {
								block.baseAddr,
								length,
								false
							};

							block.baseAddr += length;

							m_memoryBlocks.insert(
								blockIt,
								newChunk
							);

							return newChunk.baseAddr;

						}
					}
				} // for m_memoryBlocks

				throw new std::bad_alloc("No memory available to alloc");
			}

			
			void free(fpgaPtr_t addr)
			{
				auto begin = m_memoryBlocks.begin();
				auto end = m_memoryBlocks.end();

				for(auto blockIt = begin; blockIt != end; blockIt++)
				{
					if(addr == blockIt->baseAddr)
					{
						auto other = std::next(blockIt);
						bool canRemove = false;

						if(next != end && next->isFree)
						{
							next->length += blockIt->length;
							m_memoryBlocks.erase(blockIt);
						}
						else if(blockIt != begin && (other = std::prev(blockIt))->isFree)
						{
							other->length += blockIt->length;
							m_memoryBlocks.erase(blockIt);
						}
						else
						{
							blockIt->isFree = true;
						}
						break;
					}
				}
			}

		};

	}
}

#endif // _MEMORY_MANAGER_H_

#endif // FPGA
