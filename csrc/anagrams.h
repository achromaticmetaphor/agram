#ifndef ANAGRAMS_H
#define ANAGRAMS_H

#include <cstddef>

#include "agram_types.h"
#include "lcwc.h"
#include "wordlist.h"

struct agst {
  struct wc target;
  size_t offset;
  const struct lc * * wcs;
  size_t wcslen;
  const struct lc * * wcsp;
};

struct agsto {
  struct agst * states;
  size_t depth;
  agram_dchar * prefix;
  agram_cpt * chars_scratch;
  unsigned int * counts_scratch;
  const struct wordlist * wl;
};

int anagrams_init(struct agsto *, const struct wordlist *, agram_dchar const *, size_t);
size_t anagrams_single(struct agsto *);
void anagrams_destroy(struct agsto *);
int anagrams(const struct wordlist *, agram_dchar const *, size_t, int (*)(agram_dchar const *, size_t, void *), void *);

#endif
