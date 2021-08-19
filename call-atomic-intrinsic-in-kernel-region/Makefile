CXX = clang++
#nvc++ 
CC = nvcc 
#-g -G -gencode arch=compute_70,code=sm_70

CXXFLAGS = -std=c++11 -O3 -Wall -fopenmp -DUSE_OMP_ACCELERATOR
#-mp=gpu -cuda -DUSE_OMP_ACCELERATOR

OBJ = dummy.o  

all: $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS)  -o $@ $^


clean:
	rm -rf *~ $(OBJ) 
