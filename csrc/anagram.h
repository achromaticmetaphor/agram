#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <cstddef>

#include "agram_types.h"
#include "lcwc.h"
#include "wordlist.h"

template <typename CB>
int anagram(wordlist const & wl, agram_dchar const * const str,
            size_t const len, CB & cb)
{
  struct wc target(str, len);
  for (auto lc : wl.words_counts)
    if (target.is_anagram(wl, lc))
      if (cb(lc.str + wl.strbase.data(), lc.len))
        return 2;
  return 0;
}

#endif
