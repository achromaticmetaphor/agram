#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <cstddef>

#include "agram_types.h"
#include "anagram_target.h"
#include "wordlist.h"

template <typename CB>
int anagram(wordlist const & wl, agram_dchar const * const str,
            size_t const len, CB & cb)
{
  anagram_target target(str, len);
  for (auto & entry : wl.words_counts)
    if (target.is_anagram(wl, entry))
      if (cb(entry.str + wl.strbase.data(), entry.len))
        return 2;
  return 0;
}

#endif
