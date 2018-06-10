#ifndef WORDLIST_H
#define WORDLIST_H

#include <vector>

#include "agram_types.h"
#include "wordlist_entry.h"

struct wordlist
{
  std::vector<wordlist_entry> words_counts;
  std::vector<agram_display_char> strbase;
  std::vector<agram_codepoint> charsbase;
  std::vector<unsigned int> countsbase;
};

#endif
