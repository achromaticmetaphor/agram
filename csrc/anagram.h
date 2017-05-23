#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <cstddef>

#include "agram_types.h"
#include "is_anagram.h"
#include "lcwc.h"
#include "wordlist.h"

template <typename CB>
int anagram(wordlist const & wl, agram_dchar const * const str, size_t const len, CB & cb)
{
  struct wc target(str, len);
  for (agram_size i = 0; i < wl.nwords; i++)
    if (is_anagram(&wl, &target, wl.words_counts + i))
      if (cb(wl.words_counts[i].str + wl.strbase, wl.words_counts[i].len))
        return 2;
  return 0;
}

#endif
