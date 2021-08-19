#include <iostream>

__device__ int myAtomicAdd(int* address, int incr)
{
  int old = *address, assumed;

  do 
  {
    assumed = old;
    old = atomicCAS(address, assumed, incr + assumed);
  } while (assumed != old);
  
  return old;
}

template<typename T>
__device__ T fetch_and_add(T *x, T inc) 
{
    T orig_val = myAtomicAdd(x,inc);
    return orig_val;
}

__global__ void kernel(int *sharedInteger)
{
    myAtomicAdd(sharedInteger, 1);
}

//template __device__ long fetch_and_add<long>(long*, long);
//template __device__ float fetch_and_add<float>(float*, float);
//template __device__ double fetch_and_add<double>(double*, double);


template __device__ int fetch_and_add<int>(int*, int);
