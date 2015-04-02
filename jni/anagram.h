#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <stddef.h>

#include "agram_types.h"

int anagram(const agram_dchar *, size_t, int (*)(agram_dchar const *, size_t, void *), void *);

#endif
