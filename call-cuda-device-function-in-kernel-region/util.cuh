#ifndef UTIL_H_
#define UTIL_H_


__device__ int myAtomicAdd(int *address, int incr);

__global__ void kernel(int *sharedInteger);

template<typename T> __device__ T fetch_and_add(T *x, T inc);



#endif
