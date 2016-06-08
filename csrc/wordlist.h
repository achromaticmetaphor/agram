#ifndef WORDLIST_H
#define WORDLIST_H

#include "agram_types.h"

struct wordlist
{
  agram_size nwords;
  struct lc * words_counts;
  agram_dchar * strbase;
  agram_cpt * charsbase;
  unsigned int * countsbase;
  size_t words_counts_len;
  size_t strbase_len;
  size_t charsbase_len;
  size_t countsbase_len;
};

#endif
