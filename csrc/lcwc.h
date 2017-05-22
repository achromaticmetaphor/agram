#ifndef LCWC_H
#define LCWC_H

#include <cstddef>

#include "agram_types.h"
#include "wordlist.h"

struct lc
{
  unsigned int len;
  unsigned int nchars;
  size_t str;
  size_t chars;
  unsigned long int hash;
};

struct wc
{
  unsigned int len;
  unsigned int nchars;
  agram_dchar * str;
  agram_cpt * chars;
  unsigned int * counts;
  unsigned long int hash;
};

void wc_sub_s(const struct wordlist *, struct wc *, const struct wc *, const struct lc *);
int wc_sub(const struct wordlist *, struct wc *, const struct wc *, const struct lc *);
int wc_init(struct wc *, const agram_dchar *, size_t);
void wc_free(struct wc *);
unsigned long int wc_hash_chars(agram_cpt const *, unsigned int);

#endif
