#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <cstddef>

#include "agram_types.h"
#include "wordlist.h"

int anagram(const struct wordlist *, const agram_dchar *, size_t, int (*)(agram_dchar const *, size_t, void *), void *);

#endif
