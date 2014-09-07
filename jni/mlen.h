#ifndef MLEN_H
#define MLEN_H

#include <stddef.h>

#define MLEN_DECL(name, type) \
  static size_t mlen_ ## name (type const * str) \
  { \
    type const * end = str; \
    while (*end) \
      end++; \
    return end - str; \
  }

#endif
