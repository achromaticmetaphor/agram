#ifndef LCWC_H
#define LCWC_H

struct wc
{
  unsigned int len;
  unsigned int nchars;
  const char * str;
  char * chars;
  unsigned int * counts;
};

int wc_sub(struct wc *, const struct wc *, const struct wc *);
int wc_init(struct wc *, const char *);
void wc_free(struct wc *);

#endif
