#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

struct vector
{
  void * * vector;
  size_t length;
  size_t capacity;
};

void vector_init(struct vector *);
int vector_append(struct vector *, void *);
void vector_sort(struct vector *, int (*)(const void *, const void *));
void vector_traverse(struct vector *, void (*)(void *));
void vector_destroy(struct vector *);

#endif
