#include <string.h>

#include "lcwc.h"

int is_anagram(const struct wc * const a, const struct wc * const b)
{
  return a->len == b->len
      && a->nchars == b->nchars
      && ! strcmp(a->chars, b->chars)
      && ! memcmp(a->counts, b->counts, sizeof(a->counts[0]) * a->nchars);
}
