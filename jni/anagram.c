#include "agram_types.h"
#include "agram_wc.h"
#include "is_anagram.h"
#include "lcwc.h"

int anagram(const agram_dchar * const str, const size_t len, int (* const cb)(agram_dchar const *, size_t, void *), void * const cba)
{
  agram_size i;
  struct wc target;
  if (wc_init(&target, str, len))
    return 1;
  for (i = 0; i < NWORDS; i++)
    if (is_anagram(&target, words_counts+i))
      if (cb(words_counts[i].str + strbase, words_counts[i].len, cba))
        return wc_free(&target), 2;
  wc_free(&target);
  return 0;
}
