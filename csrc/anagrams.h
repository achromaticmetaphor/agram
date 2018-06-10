#ifndef ANAGRAMS_H
#define ANAGRAMS_H

#include <cstddef>
#include <functional>
#include <vector>

#include "agram_types.h"
#include "anagram_target.h"
#include "optional.h"
#include "string_view.h"
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

  agsto(wordlist const &, string_view<agram_display_char>);
  optional<string_view<agram_display_char>> single();
};

template <typename CB>
int anagrams(wordlist const & wl, string_view<agram_display_char> sv, CB & cb)

{
  agsto ostate(wl, sv);
  while (auto next = ostate.single())
    if (cb(*next))
      return 2;
  return 0;
}

#endif
