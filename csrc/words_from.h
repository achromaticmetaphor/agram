#ifndef WORDS_FROM_H
#define WORDS_FROM_H

#include <algorithm>
#include <cstddef>
#include <limits>

#include "agram_types.h"
#include "anagram_target.h"
#include "wordlist.h"

template <typename CB>
int words_from(wordlist const & wl, string_view<agram_display_char> sv,
               int const max, CB & cb)
{
  anagram_target target(sv);
  if (max)
    std::fill(target.counts.begin(), target.counts.end(),
              std::numeric_limits<unsigned int>::max());
  for (auto & entry : wl.words_counts)
    if (target.contains(wl, entry))
      if (cb(wl.display_string(entry)))
        return 2;
  return 0;
}

#endif
