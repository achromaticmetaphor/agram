#include <stdlib.h>

#include "vector.h"

void vector_init(struct vector * const vec)
{
  vec->vector = NULL;
  vec->length = 0;
  vec->capacity = 0;
}

int vector_append(struct vector * const vec, void * const val)
{
  if (vec->length >= vec->capacity)
    {
      void * const nvec = realloc(vec->vector, sizeof(*vec->vector) * (vec->capacity * 2 + 1));
      if (! nvec)
        return 1;
      vec->vector = nvec;
      vec->capacity = vec->capacity * 2 + 1;
    }
  vec->vector[vec->length++] = val;
  return 0;
}

void vector_sort(struct vector * const vec, int (* const cmp)(const void *, const void *))
{
  qsort(vec->vector, vec->length, sizeof(*vec->vector), cmp);
}

void vector_traverse(struct vector * const vec, void (* const cb)(void *))
{
  for (size_t i = 0; i < vec->length; i++)
    cb(vec->vector[i]);
}

void vector_destroy(struct vector * const vec)
{
  free(vec->vector);
}
