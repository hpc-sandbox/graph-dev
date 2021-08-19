#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <utility>
#include <cassert>
#include <omp.h>

using namespace std;

#ifndef SIZE
#define SIZE (200)
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
    
  T* array_;
  int n_; 
};
	
void nbrscan_atomic(Dummy<int>* d)
{
  const int n_ = d->n_;
  int* atarr;
  int compare = 5;
  int val = 6;
  bool week = false;
  int success_order = 1;
  int failure_order = 0;

  atarr = (int*)malloc(n_*sizeof(int));
  memset(&atarr, 0, n_*sizeof(int));

#pragma omp target teams distribute parallel for \
       map(tofrom:atarr[0:n_])
  for (int i = 0; i < n_; i++)
  {
      atarr[i] += 1;
     if(__atomic_compare_exchange(&atarr[i], &compare, &val, week,success_order, failure_order)){
     // std::cout << "---Fail----" << atarr[n_-1] << std::endl;
     }
  }
    
  std::cout << "---Success----" << atarr[n_-1] << std::endl;
  free(atarr);
}

/**
void nbrscan_atomic2(Dummy<int>* d)
{
  const long n_ = d->n_;
  int*atarr, *atarr_dev;
  int compare = 5;
  int val = 6;
  bool week = false;
  int success_order = 1;
  int failure_order = 0;

  
  int h = omp_get_initial_device();
  int t = omp_get_default_device();

  if (omp_get_num_devices() < 1 || t < 0)
  {
    std::cout << " ERROR: No device found.\n";
    std::abort();
  }

  atarr = (int*)malloc(n_*sizeof(int));
  memset(atarr, 0, sizeof(int)*n_);
  
  atarr_dev = (int*)omp_target_alloc(sizeof(int)*n_, t);
  omp_target_memcpy(atarr_dev, atarr, sizeof(int)*n_, 0, 0, t, h);

#pragma omp target teams distribute parallel for \
  is_device_ptr(atarr_dev)
  for (int i = 0; i < n_; i++)
  {
      atarr[i] += 1;
      //int res = atomicCAS(atarr[i],compare,val);
      //int res =  fetch_and_add<int>(&atarr_dev[i], 1);
      if(__atomic_compare_exchange(&atarr[i], &compare, &val, week,success_order, failure_order)){
      // std::cout << "---Fail----" << atarr[n_-1] << std::endl;
      } 
  }

  omp_target_memcpy(atarr, atarr_dev, sizeof(int)*n_, 0, 0, h, t);
  omp_target_free(atarr_dev, t);
  std::cout <<"---Success----" << atarr[n_-1] << std::endl;
  free(atarr);
}
*/

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
    n = SIZE;
  }
  
  std::cout << "Size: " << n << std::endl;
  Dummy<int> *d = new Dummy<int>(n);

  std::cout << "Access mapped array.\n";
  nbrscan_atomic(d);
  std::cout << "Access device pointer.\n";
 // nbrscan_atomic2(d);

  return 0;
}
