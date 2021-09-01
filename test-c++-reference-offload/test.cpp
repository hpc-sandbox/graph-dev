#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <utility>
#include <cassert>
#include <algorithm>

#include <omp.h>

using namespace std;

#ifndef SIZE
#define SIZE (200)
#endif

struct Data
{
  int i_;
  double d_;
  Data(): i_(0), d_(0.0) {}
  Data(int i, double d): i_(i), d_(d) {}
}; 

template <typename T>
class Incrementer
{
  public:
    Incrementer(): i_(0) {}
    T operator()() { return i_++; }
  private:
    T i_;
};

template class Incrementer<double>;

class Dummy
{
	public:
    Dummy(int n): 
      array_(nullptr), n_(n)
    {
      array_ = new Data[n_];
      for (int i = 0; i < n_; i++)
      {
        array_[i].i_ = i;
        array_[i].d_ = (double)(i + 1);
      }
#pragma omp target enter data map(to:this[:1])
#pragma omp target enter data map(alloc:array_[0:n_])
    }
  
   ~Dummy() 
   { 
#pragma omp target exit data map(delete:array_[0:n_])
     delete[] array_; 
   }

   Dummy(const Dummy &d) = delete;
   Dummy& operator=(const Dummy& d) = delete;
   
   inline void max(Data& max_data) 
   {
     for (int i = 0; i < n_; i++)
     {
       if (array_[i].d_ > max_data.d_)
         max_data = array_[i];

       if (array_[i].d_ == max_data.d_)
       {
         if (array_[i].i_ > max_data.i_)
           max_data = array_[i];
       }
     } 
   }

   Data* array_;
  private:   
   int n_; 
};


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
    
  double *array2 = new double[n];
  std::generate(array2, array2 + n, Incrementer<double>());

#pragma omp target enter data map(alloc:array2[0:n])

  Dummy d(n);
 
#pragma omp target teams distribute parallel for \
  map(always, tofrom: array2[0:n]) \
  map(always, to: d.array_[0:n]) 
  for (int i = 0; i < n; i++)
  {
    Data max_data;
    d.max(max_data);
    array2[i] += max_data.d_;
  }

  std::cout << "---Success----" << array2[n-1] << std::endl;
#pragma omp target exit data map(delete:array2[0:n])
  
  delete []array2;
  
  return 0;
}
