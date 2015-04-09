#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

struct vector
{
  char * vector;
  size_t length;
  size_t capacity;
};

void vector_init(struct vector *);
int vector_append(struct vector *, void const *, size_t);
void vector_sort(struct vector *, size_t, int (*)(const void *, const void *));
void vector_traverse(struct vector *, size_t, void (*)(void *));
void vector_destroy(struct vector *);

#endif
