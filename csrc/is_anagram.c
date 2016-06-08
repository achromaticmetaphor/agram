#define _POSIX_C_SOURCE 200809L

#include <string.h>

#include "lcwc.h"
#include "wordlist.h"

int is_anagram(const struct wordlist * const wl, const struct wc * const a, const struct lc * const b)
{
  return a->len == b->len
      && a->nchars == b->nchars
      && ! memcmp(a->chars, wl->charsbase + b->chars, sizeof(a->chars[0]) * a->nchars)
      && ! memcmp(a->counts, wl->countsbase + b->chars, sizeof(a->counts[0]) * a->nchars);
}
