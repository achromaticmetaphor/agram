#ifndef WORDLIST_ENTRY_H
#define WORDLIST_ENTRY_H

#include <cstddef>
#include <numeric>
#include <vector>

#include "agram_types.h"
#include "wordlist.h"

struct wordlist_entry
{
  unsigned int len;
  unsigned int nchars;
  size_t str;
  size_t chars;
  unsigned long int hash;
};

#endif
