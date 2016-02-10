#ifndef WORDS_FROM_H
#define WORDS_FROM_H

#include <stddef.h>

#include "agram_types.h"
#include "wordlist.h"

int words_from(const struct wordlist *, agram_dchar const *, size_t, int, int (*)(agram_dchar const *, size_t, void *), void *);

#endif
