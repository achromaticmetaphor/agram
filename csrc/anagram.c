#define _POSIX_C_SOURCE 200809L

#include "agram_types.h"
#include "is_anagram.h"
#include "lcwc.h"
#include "wordlist.h"

int anagram(const struct wordlist * const wl, const agram_dchar * const str, const size_t len, int (* const cb)(agram_dchar const *, size_t, void *), void * const cba)
{
  agram_size i;
  struct wc target;
  if (wc_init(&target, str, len))
    return 1;
  for (i = 0; i < wl->nwords; i++)
    if (is_anagram(wl, &target, wl->words_counts+i))
      if (cb(wl->words_counts[i].str + wl->strbase, wl->words_counts[i].len, cba))
        return wc_free(&target), 2;
  wc_free(&target);
  return 0;
}
