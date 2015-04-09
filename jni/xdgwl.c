#include <stddef.h>
#include <string.h>

#include "wc.h"
#include "wordlist.h"

static int has_next(void * vx)
{
  return ! ! wordlist[++(* (int *) vx)];
}

static size_t len(void * vx)
{
  return strlen(wordlist[* (int*) vx]);
}

static void get(agram_dchar * out, void * vx)
{
  strcpy(out, wordlist[* (int *) vx]);
}

int init_wl(void)
{
  static const struct cwlcbs cbs = {has_next, len, get};
  int x = -1;
  return build_wl(&cbs, &x);
}
