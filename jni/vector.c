#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>

#include "vector.h"

void vector_init(struct vector * const vec)
{
  vec->vector = NULL;
  vec->length = 0;
  vec->capacity = 0;
}

int vector_append(struct vector * const vec, void const * const val, size_t size)
{
  size_t ncap = vec->capacity;
  while (vec->length + size >= ncap)
    ncap = ncap * 2 + 1;
  if (ncap != vec->capacity)
    {
      void * const nvec = realloc(vec->vector, sizeof(*vec->vector) * ncap);
      if (! nvec)
        return 1;
      vec->vector = nvec;
      vec->capacity = ncap;
    }
  memcpy(vec->vector + vec->length, val, size);
  vec->length += size;
  return 0;
}

void vector_sort(struct vector * const vec, size_t size, int (* const cmp)(const void *, const void *))
{
  qsort(vec->vector, vec->length / size, size, cmp);
}

void vector_traverse(struct vector * const vec, size_t size, void (* const cb)(void *))
{
  size_t i;
  for (i = 0; i < vec->length; i += size)
    cb(vec->vector + i);
}

void vector_destroy(struct vector * const vec)
{
  free(vec->vector);
}
