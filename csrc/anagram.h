#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <cstddef>

#include "agram_types.h"
#include "anagram_target.h"
#include "string_view.h"
#include "wordlist.h"

template <typename CB>
int anagram(wordlist const & wl, string_view<agram_display_char> sv, CB & cb)
{
  anagram_target target(sv);
  for (auto & entry : wl.words_counts)
    if (target.is_anagram(wl, entry))
      if (cb(wl.display_string(entry)))
        return 2;
  return 0;
}

#endif
