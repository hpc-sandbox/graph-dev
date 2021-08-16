#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <utility>
#include <cassert>
#include <omp.h>
#include "device_launch_parameters.h"

#include "util.cuh"

using namespace std;

#ifndef NV
#define NV (200)
#endif

template<typename T>
class Dummy
{
	public:
    Dummy(int n): n_(n)
    {
      array_ = new T[n_];
      memset(array_, 0, sizeof(T)*n_);
    }
    
   ~Dummy()
    {
      delete[] array_;
    }
    
//  private:
  T* array_;
  int n_; 
};
	
void nbrscan_atomic (Dummy<int>* d)
{
  const int n_ = d->n_;
  int* atarr;
  int p = 20;
  atarr = (int*)malloc(n_*sizeof(int));
  memset(&atarr, 0, n_*sizeof(int));

#pragma omp target teams distribute parallel for\
        map(tofrom:atarr[0:n_])
  for (int i = 0; i < n_; i++)
  {
      atarr[i] += 1;
      //kernel<<<1,1>>>(&atarr[i]);
      //myAtomicAdd(&atarr[i], p);
      int res =  fetch_and_add<int>(&atarr[i], p);

  }
    std::cout <<"---value update-----" << atarr[n_-1] << std::endl;
}



int main(int argc, char *argv[])
{
  int n;
  if (argc == 2)
  {
    n = atol(argv[1]);
  }
  else if (argc == 3)
  {
    n = atol(argv[1]);
  }
  else
  {
    n = NV;
  }
  
  std::cout << "#Vertices: " << n << std::endl;
  Dummy<int> *d = new Dummy<int>(n);
  
  //d.print_edges();
  // d.nbrscan();
 // nbrscan_free(d);
 nbrscan_atomic(d);

 std::cout << "Scan neighborhood and copy" << std::endl;
  
  return 0;
}
