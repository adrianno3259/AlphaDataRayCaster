
#################################
#       COMPILATION TOOLS 	    #
#################################

#final executable file
EXEC=raytracer

#compiler
CC=g++ -std=c++11

# linker
LINKER=g++


#################################
#		PROJECT DIRECTORIES		#
#################################

# sources directory
SRC_DIR=src

# object directory
OBJS_DIR=obj

# include directory
HEADER_DIR=include

# external libs and dependencies directory
DEP_DIR=deps

MAIN=main.cpp

SOURCES = $(MAIN) $(wildcard $(SRC_DIR)/*.cpp)

OBJS= $(patsubst %.cpp, %.o, $(SOURCES))

HEADERS= $(wildcard include/*.h)

DEPS=$(patsubst %.cpp, %.d, $(SOURCES))

#################################
#		COMPILATION OPTIONS		#
#################################

# Include drectory options
INC_DIR = -I$(HEADER_DIR) 

ifeq ($(FPGA), true)
$(info LOG: FPGA usage defined)
INC_DIR+= -I/home/fpgadev/Documents/Adrianno/admpcieku3_sdk-2.0.0/host/api-v1_4_18b9/include
CPP_FLAGS+= -DFPGA
LINK_FLAGS+= -ladmxrc3
else	
#$(info LOG: FPGA usage not defined)
LINK_FLAGS=
endif


CPP_FLAGS= $(INC_DIR)

target : $(EXEC)

run: $(EXEC) clean_intermediate
	./$(EXEC)

$(EXEC) : $(OBJS)
	$(LINKER) $^ -o $@ $(LINK_FLAGS)

%.o: %.cpp
	$(CC) $(CPP_FLAGS) -c $< -o $@

%.d: %.cpp
	$(CC) $(INC_DIR) -MM $< > $@

clean_full: clean_intermediate
	rm $(EXEC)

clean:
	rm $(OBJS) $(DEPS)

-include $(DEPS)
