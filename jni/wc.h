#ifndef WC_H
#define WC_H

#include <stddef.h>

#include "agram_types.h"

struct cwlcbs
{
  int (* has_next)(void *);
  size_t (* len)(void *);
  void (* get)(agram_dchar *, void *);
};

int compile_wl(const char *, struct cwlcbs const *, void *);

#endif
