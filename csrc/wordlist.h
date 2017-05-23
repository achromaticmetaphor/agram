#ifndef WORDLIST_H
#define WORDLIST_H

#include <vector>

#include "agram_types.h"
struct lc;

struct wordlist
{
  std::vector<lc> words_counts;
  std::vector<agram_dchar> strbase;
  std::vector<agram_cpt> charsbase;
  std::vector<unsigned int> countsbase;
};

#endif
