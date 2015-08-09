#ifndef LCWC_H
#define LCWC_H

#include <stddef.h>

#include "agram_types.h"

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

void wc_sub_s(struct wc *, const struct wc *, const struct lc *);
int wc_sub(struct wc *, const struct wc *, const struct lc *);
int wc_init(struct wc *, const agram_dchar *, size_t);
void wc_free(struct wc *);
unsigned long int wc_hash_chars(agram_cpt const *, unsigned int);

#endif
