#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <stddef.h>

#include <jni.h>

int anagram(const jchar *, size_t, int (*)(jchar const *, size_t, void *), void *);

#endif
