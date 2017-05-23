#ifndef WORDS_FROM_H
#define WORDS_FROM_H

#include <cstddef>
#include <limits>

#include "agram_types.h"
#include "lcwc.h"
#include "wordlist.h"

template <typename CB>
int words_from(wordlist const & wl, agram_dchar const * const str, size_t const len, int const max, CB & cb)
{
  struct wc target(str, len);
  if (max)
    for (auto & c : target.counts)
      c = std::numeric_limits<unsigned int>::max();
  for (auto lc : wl.words_counts)
    if (target.contains(wl, lc))
      if (cb(lc.str + wl.strbase.data(), lc.len))
        return 2;
  return 0;
}

#endif
