#ifndef WORDS_FROM_H
#define WORDS_FROM_H

#include <stddef.h>
#include <jni.h>

int words_from(jchar const *, size_t, int, int (*)(jchar const *, size_t, void *), void *);

#endif
