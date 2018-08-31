#define CATCH_CONFIG_MAIN
#include<catch2/catch.hpp>
//#include <single_include/catch2/catch.hpp>
#include "accelfpga.hpp"
#include <iostream>


TEST_CASE("teste_mem")
{

    fpga::MemoryManager manager(8e9);



    std::cout << "teste\n\n";
}
