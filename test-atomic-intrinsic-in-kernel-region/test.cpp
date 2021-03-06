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
  
   /* 
   Dummy(const Dummy &d) 
   {
     n_ = d.n_;
     memcpy(array_, d.array_, sizeof(T)*n_); 
   } 
   */

   Dummy(const Dummy &d) = delete;
   Dummy& operator=(const Dummy& d) = delete;

   void atomic_test()
   {
     int compare = 5;
     int val = 6;
     bool week = false;
     int success_order = 1;
     int failure_order = 0;

#pragma omp target teams distribute parallel for 
     for (int i = 0; i < n_; i++)
     {
       array_[i] = i;
       __atomic_compare_exchange(&array_[i], &compare, &val, week, success_order, failure_order);
     }
   }

  //private:   
   T* array_;
   int n_; 
};

template class Dummy<int>;

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
  const int x = d->n_;

#pragma omp target enter data map(to:d[:1])
#pragma omp target enter data map(alloc:d->array_[0:x])
  d->atomic_test();
#pragma omp target exit data map(from:d->array_[0:x])
     
  std::cout << "---Success----" << d->array_[x-1] << std::endl;

  delete d;

  return 0;
}
