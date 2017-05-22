#ifndef WORDS_FROM_H
#define WORDS_FROM_H

#include <cstddef>

#include "agram_types.h"
#include "wordlist.h"

int words_from(const struct wordlist *, agram_dchar const *, size_t, int, int (*)(agram_dchar const *, size_t, void *), void *);

#endif
