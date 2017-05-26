#ifndef WC_H
#define WC_H

#include <cstddef>

#include "agram_types.h"
#include "lcwc.h"
#include "wordlist.h"

struct cwlsrc
{
  virtual ~cwlsrc() {}
  virtual int has_next() = 0;
  virtual size_t len() = 0;
  virtual agram_dchar const * get() = 0;

  int compile_wl(const char *);
  int build_wl(struct wordlist *);
};

#endif
