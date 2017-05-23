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
  for (agram_size i = 0; i < wl.nwords; i++)
    if (target.contains(wl, wl.words_counts[i]))
      if (cb(wl.words_counts[i].str + wl.strbase, wl.words_counts[i].len))
        return 2;
  return 0;
}

#endif
