#ifndef ANAGRAMS_H
#define ANAGRAMS_H

#include <stddef.h>

#include "agram_types.h"
#include "lcwc.h"

struct agst {
  struct wc target;
  size_t offset;
  const struct lc * * wcs_in;
  const struct lc * * wcs;
  size_t wcslen;
  const struct lc * * wcsp;
};

struct agsto {
  struct agst * states;
  size_t depth;
  agram_dchar * prefix;
  int error;
};

int anagrams_init(struct agsto *, agram_dchar const *, size_t);
size_t anagrams_single(struct agsto *);
void anagrams_destroy(struct agsto *);
int anagrams(agram_dchar const *, size_t, int (*)(agram_dchar const *, size_t, void *), void *);

#endif
