#ifndef WORDLIST_SOURCE_H
#define WORDLIST_SOURCE_H

#include <cstddef>

#include "agram_types.h"
#include "wordlist.h"

struct wordlist_source
{
  virtual ~wordlist_source() {}
  virtual int has_next() = 0;
  virtual size_t len() = 0;
  virtual agram_display_char const * get() = 0;

  int compile_wl(const char *);
  int build_wl(wordlist *);
};

#endif
