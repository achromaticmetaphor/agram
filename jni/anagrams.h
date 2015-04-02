#ifndef ANAGRAMS_H
#define ANAGRAMS_H

#include <stddef.h>

#include "agram_types.h"

int anagrams(agram_dchar const *, size_t, int (*)(agram_dchar const *, size_t, void *), void *);

#endif
