#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <algorithm>
#include <cstddef>
#include <ostream>

template <typename T> std::size_t null_len(T const * str)
{
  T const * const start = str;
  while (*str)
    ++str;
  return str - start;
}

template <typename T> struct string_view
{
  T const * data = nullptr;
  std::size_t const len = 0;

  string_view() : data{nullptr}, len{0} {}
  string_view(T const * str) : data{str}, len{null_len(str)} {}
  string_view(T const * str, std::size_t slen) : data{str}, len{slen} {}
};

static inline std::ostream & operator<<(std::ostream & out,
                                        string_view<char> const & sv)
{
  std::for_each(sv.data, sv.data + sv.len, [&out](char c) { out << c; });
  return out;
}

#endif
