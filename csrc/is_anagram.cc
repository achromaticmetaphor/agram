#define _POSIX_C_SOURCE 200809L

#include <cstring>

#include "lcwc.h"
#include "wordlist.h"

int is_anagram(wordlist const * const wl, wc const * const a, lc const * const b)
{
  if (a->len != b->len)
    return 0;
  if (a->chars.size() != b->nchars)
    return 0;
  for (unsigned int i = 0; i < b->nchars; ++i)
    {
      if (a->chars[i] != (wl->charsbase + b->chars)[i])
        return 0;
      if (a->counts[i] != (wl->countsbase + b->chars)[i])
        return 0;
    }
  return 1;
}
