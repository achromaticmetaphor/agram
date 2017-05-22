#ifndef ANAGRAMS_H
#define ANAGRAMS_H

#include <cstddef>
#include <vector>

#include "agram_types.h"
#include "lcwc.h"
#include "wordlist.h"

struct agst
{
  wc target;
  size_t offset;
  std::vector<lc *> wcs;
  size_t wcsi;

  lc * const & current() const { return wcs[wcsi]; }
  lc *& advance() { return wcs[wcsi++]; }
  bool exhausted() const { return wcsi >= wcs.size(); }
};

struct agsto
{
  std::vector<agst> states;
  size_t depth;
  std::vector<agram_dchar> prefix;
  wordlist const & wl;

  agsto(wordlist const &, agram_dchar const *, size_t);
  size_t single();
};

int anagrams(wordlist const &, agram_dchar const *, size_t, int (*)(agram_dchar const *, size_t, void *), void *);

#endif
