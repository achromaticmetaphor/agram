#ifndef LCWC_H
#define LCWC_H

#include <stddef.h>

#include <jni.h>

struct lc
{
  unsigned int len;
  unsigned int nchars;
  size_t str;
  size_t chars;
};

struct wc
{
  unsigned int len;
  unsigned int nchars;
  const jchar * str;
  jint * chars;
  unsigned int * counts;
};

int wc_sub(struct wc *, const struct wc *, const struct lc *);
int wc_init(struct wc *, const jchar *, jsize);
void wc_free(struct wc *);
void lcwc(struct wc *, const struct lc *);

#endif
