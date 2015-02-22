#include <string.h>

#include "agram_wc.h"
#include "lcwc.h"

int is_anagram(const struct wc * const a, const struct lc * const b)
{
  return a->len == b->len
      && a->nchars == b->nchars
      && ! memcmp(a->chars, charsbase + b->chars, sizeof(a->chars[0]) * a->nchars)
      && ! memcmp(a->counts, countsbase + b->chars, sizeof(a->counts[0]) * a->nchars);
}
