#ifndef ANAGRAMS_H
#define ANAGRAMS_H

#include <stddef.h>

#include <jni.h>

int anagrams(jchar const *, size_t, int (*)(jchar const *, size_t, void *), void *);

#endif
