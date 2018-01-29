#final executable file
EXEC=raytracer

#compiler
CC=g++ -std=c++11

#linker
LINKER=g++

# sources directory
SRC_DIR=src
# objec 
OBJS_DIR=obj
HEADER_DIR=include
DEP_DIR=deps

INC_DIR = -I$(HEADER_DIR)

CPP_FLAGS= 

SOURCES = main.cpp $(wildcard $(SRC_DIR)/*.cpp)

OBJS= $(patsubst %.cpp, %.o, $(SOURCES))

HEADERS= $(wildcard include/*.h)

DEPS=$(patsubst %.cpp, %.d, $(SOURCES))

run: $(EXEC) clean_intermediate
	./$(EXEC)

$(EXEC) : $(OBJS)
	$(LINKER) $^ -o $@

%.o: %.cpp
	$(CC) $(CPP_FLAGS) $(INC_DIR) -c $< -o $@

%.d: %.cpp
	$(CC) $(INC_DIR) -MM $< > $@

clean_full: clean_intermediate
	rm $(EXEC)

clean_intermediate: 
	rm $(OBJS) $(DEPS)

-include $(DEPS)
