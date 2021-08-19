CXX = nvc++
CC = nvcc

CXXFLAGS = -mp=gpu -cuda 
#-DUSE_OMP_ACCELERATOR

OBJ = test.o util.o 
TARGET = bin

all: $(TARGET)

%.o: %.cu
	$(CC) -dc -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

$(TARGET):  $(OBJ)
	$(CXX) -cuda $^ -o $@

clean:
	rm -rf *~ $(OBJ) $(TARGET)
