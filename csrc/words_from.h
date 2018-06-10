#ifndef WORDS_FROM_H
#define WORDS_FROM_H

#include <cstddef>
#include <limits>

#include "agram_types.h"
#include "anagram_target.h"
#include "wordlist.h"

template <typename CB>
int words_from(wordlist const & wl, agram_display_char const * const str,
               size_t const len, int const max, CB & cb)
{
  anagram_target target(str, len);
  if (max)
    for (auto & c : target.counts)
      c = std::numeric_limits<unsigned int>::max();
  for (auto & entry : wl.words_counts)
    if (target.contains(wl, entry))
      if (cb(entry.str + wl.strbase.data(), entry.len))
        return 2;
  return 0;
}

#endif
