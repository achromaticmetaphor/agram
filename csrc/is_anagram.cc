#define _POSIX_C_SOURCE 200809L

#include <cstring>

#include "lcwc.h"
#include "wordlist.h"

int is_anagram(wordlist const * const wl, wc const * const a, lc const * const b)
{
  return a->len == b->len
      && a->nchars == b->nchars
      && ! memcmp(a->chars, wl->charsbase + b->chars, sizeof(a->chars[0]) * a->nchars)
      && ! memcmp(a->counts, wl->countsbase + b->chars, sizeof(a->counts[0]) * a->nchars);
}
