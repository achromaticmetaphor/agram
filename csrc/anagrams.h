#ifndef ANAGRAMS_H
#define ANAGRAMS_H

#include <cstddef>
#include <functional>
#include <vector>

#include "agram_types.h"
#include "anagram_target.h"
#include "wordlist.h"

struct agst
{
  anagram_target target;
  size_t offset;
  std::vector<std::reference_wrapper<wordlist_entry const>> wcs;
  size_t wcsi;

  wordlist_entry const & current() const { return wcs[wcsi]; }
  wordlist_entry const & advance() { return wcs[wcsi++]; }
  bool exhausted() const { return wcsi >= wcs.size(); }
};

struct agsto
{
  std::vector<agst> states;
  size_t depth;
  std::vector<agram_display_char> prefix;
  wordlist const & wl;

  agsto(wordlist const &, agram_display_char const *, size_t);
  size_t single();
};

template <typename CB>
int anagrams(wordlist const & wl, agram_display_char const * const str,
             size_t const len, CB & cb)
{
  struct agsto ostate(wl, str, len);
  while (size_t slen = ostate.single())
    if (cb(ostate.prefix.data() + 1, slen))
      return 2;
  return 0;
}

#endif
