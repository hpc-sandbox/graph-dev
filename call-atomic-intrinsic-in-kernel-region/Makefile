CXX = clang++
#nvc++ 

CXXFLAGS = -std=c++11 -O3 -Wall -fopenmp -fopenmp-targets=nvptx64
#-mp=gpu -cuda -DUSE_OMP_ACCELERATOR

BIN = test 

all: $(BIN)

%.o: %.cpp
	$(CXX) $(CXXFLAGS)  -o $@ $^


clean:
	rm -rf *~ $(BIN) 
