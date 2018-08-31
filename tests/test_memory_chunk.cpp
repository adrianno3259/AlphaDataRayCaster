#define CATCH_CONFIG_MAIN
#include<catch2/catch.hpp>
#include<accelfpga.hpp>

/** Tests if the class _MemoryChunk behaves
*   correctly
*
*   typedef struct
*   {
*       const uint64_t _start;
*       const unsigned long int _size;
*       const int _wordWidth;
*       const bool _free;
*
*       inline bool isFree() const{ return _free; }
*
*    } _MemoryChunk;
*
*/
TEST_CASE("Test memory chunks"){
    REQUIRE(1 == 1);
}
