#define _POSIX_C_SOURCE 200809L

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "anagram.h"
#include "anagrams.h"
#include "vector.h"
#include "wc.h"
#include "wordlist.h"
#include "words_from.h"
#include "xdgwl.h"

static int prn(const agram_dchar * str, size_t len, void * vfd)
{
  agram_dchar ostr[len+2];
  memcpy(ostr, str, len * sizeof(*ostr));
  ostr[len] = '\n';
  ostr[len+1] = 0;
  return fputs(ostr, vfd) == EOF;
}

static int usage(const char * pn)
{
  fprintf(stderr, "usage: %s <single|multi|random|contained> <string>\n", pn);
  return 1;
}

static int single(const struct wordlist * const wl, int argc, char * argv[])
{
  if (argc == 3)
    return anagram(wl, argv[2], strlen(argv[2]), prn, stdout);
  return usage(argv[0]);
}

static int multi(const struct wordlist * const wl, int argc, char * argv[])
{
  if (argc == 3)
    return anagrams(wl, argv[2], strlen(argv[2]), prn, stdout);
  return usage(argv[0]);
}

static int random(const struct wordlist * const wl, int argc, char * argv[])
{
  const long int count = argc >= 3 ? atol(argv[2]) : 1;
  struct timespec tv;
  clock_gettime(CLOCK_REALTIME, &tv);
  srand(tv.tv_sec ^ tv.tv_nsec);
  for (long int i = 0; i < count; i++)
    {
      const int n = rand() % wl->nwords;
      if (prn(wl->words_counts[n].str + wl->strbase, wl->words_counts[n].len, stdout))
        return 1;
    }
  return 0;
}

static int contained(const struct wordlist * const wl, int argc, char * argv[])
{
  if (argc >= 3)
    return words_from(wl, argv[2], strlen(argv[2]), argc >= 4, prn, stdout);
  return usage(argv[0]);
}

int main(int argc, char * argv[])
{
  setlocale(LC_ALL, "");

  struct wordlist wl;
  if (init_wl(&wl))
    return 1;
  if (argc < 2)
    return usage(argv[0]);

  switch(argv[1][0])
    {
      case 's':
        return single(&wl, argc, argv);
      case 'm':
        return multi(&wl, argc, argv);
      case 'r':
        return random(&wl, argc, argv);
      case 'c':
        return contained(&wl, argc, argv);
      default:
        return usage(argv[0]);
    }
}
