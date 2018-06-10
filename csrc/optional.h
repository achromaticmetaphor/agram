#ifndef OPTIONAL_H
#define OPTIONAL_H

template <typename T> struct optional
{
  bool has_value;
  T value;

  optional() : has_value{false} {}

  template <typename... A> optional(A... a) : has_value{true}, value{a...} {}

  explicit operator bool() const { return has_value; }
  T const & operator*() const { return value; }
};

#endif
