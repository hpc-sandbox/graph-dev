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
    Dummy(int n): 
      n_(n), array_(nullptr), cmp_(5), 
      val_(6), sord_(1), ford_(0), wk_(false)
    {
      array_ = new T[n_];
#pragma omp target enter data map(to:this[:1])
#pragma omp target enter data map(alloc:array_[0:n_])
    }
  
   ~Dummy() 
   { 
#pragma omp target exit data map(delete:array_[0:n_])
     delete[] array_; 
   }
   /*
   Dummy(const Dummy &d) 
   {
     n_ = d.n_;
     cmp_ = d.cmp_;
     val_ = d.val_;
     wk_ = d.wk_;
     sord_ = d.sord_;
     ford_ = d.ford_;
     memcpy(array_, d.array_, sizeof(T)*n_); 
   }
   */
   Dummy(const Dummy &d) = delete;
   Dummy& operator=(const Dummy& d) = delete;
   
   void atomic(int const& i)
   { __atomic_compare_exchange(&array_[i], &cmp_, &val_, wk_, sord_, ford_); }

   int get(int const& i) const { return array_[i]; }
   
   void set(int const& i, int val) { array_[i] = val; }

  private:   
   int n_; 
   T* array_;
   int cmp_, val_, sord_, ford_;
   bool wk_; 
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
  
  int *array2 = new int[n];
  for (int i = 0; i < n; i++)
    array2[i] = i;

  Dummy<int> d(n);
 
#pragma omp target enter data map(alloc:array2[0:n])

#pragma omp target teams distribute parallel for \
  map(always, tofrom: array2[0:n]) 
  for (int i = 0; i < n; i++)
  {
    d.set(i,i);
    d.atomic(i);
    array2[i] += d.get(i);
  }

  std::cout << "---Success----" << array2[n-1] << std::endl;
#pragma omp target exit data map(delete:array2[0:n])
  
  delete []array2;
  
  return 0;
}
