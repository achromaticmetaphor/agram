#define _POSIX_C_SOURCE 200809L

#include <limits>

#include "agram_types.h"
#include "is_within.h"
#include "lcwc.h"
#include "wordlist.h"

int words_from(const struct wordlist * const wl, agram_dchar const * const str, size_t const len, int const max, int (*const cb)(agram_dchar const *, size_t, void *), void * const cba)
{
  agram_size i;
  struct wc target(str, len);
  if (max)
    {
      unsigned int i;
      for (auto & c : target.counts)
        c = std::numeric_limits<unsigned int>::max();
    }
  for (i = 0; i < wl->nwords; i++)
    if (is_within_lw(wl, wl->words_counts + i, &target))
      if (cb(wl->words_counts[i].str + wl->strbase, wl->words_counts[i].len, cba))
        return 2;
  return 0;
}
