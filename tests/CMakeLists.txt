cmake_minimum_required(VERSION 2.8)

project (ADMRAY_Tests)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Setting the bin folder to hold the executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# adding the include folder of the main project
include_directories (${PROJECT_BINARY_DIR}//..//include)

# include the FPGA folder for testing the custom class
include_directories (${PROJECT_BINARY_DIR}//..//FPGA)

#include_directories (${PROJECT_BINARY_DIR}/../../Catch2/)

# adding the test project include folder (basically for the test framework)
 include_directories (${PROJECT_BINARY_DIR}//include)


#############################################
###    Adding unit tests' executables	  ###
#############################################

# memory chunk test
 add_executable (test_memory_chunk test_memory_chunk.cpp)

# memory manager test
add_executable (test_memory_manager test_memory_manager.cpp)



