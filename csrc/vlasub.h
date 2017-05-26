#ifndef VLASUB_H
#define VLASUB_H

#include <cstddef>

template <typename T>
struct vlasub
{
  T * data;

  T & operator[](size_t n) { return data[n]; }
  T const & operator[](size_t n) const { return data[n]; }
  operator T *() { return data; }

  vlasub(size_t n) { data = new T[n]; }
  ~vlasub() { delete[] data; }
};

#endif
