#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "agram_wc.h"
#include "anagram.h"
#include "anagrams.h"
#include "vector.h"
#include "wc.h"
#include "words_from.h"
#include "xdgwl.h"

static int prn(const agram_dchar * str, size_t len, void * vfd)
{
  return putout(* (int *) vfd, str, len * sizeof(*str)) || putout(* (int *) vfd, "\n", 1);
}

static int stdoutfd = STDOUT_FILENO;

static int usage(const char * pn)
{
  fprintf(stderr, "usage: %s <single|multi|random|contained> <string>\n", pn);
  return 1;
}

static int single(int argc, char * argv[])
{
  if (argc == 3)
    return anagram(argv[2], strlen(argv[2]), prn, &stdoutfd);
  return usage(argv[0]);
}

static int append(const agram_dchar * str, size_t len, void * vec)
{
  agram_dchar * nstr = malloc((len + 1) * sizeof(*str));
  if (! nstr)
    return 1;
  memcpy(nstr, str, len * sizeof(*str));
  nstr[len] = 0;
  return vector_append(vec, nstr) ? free(nstr), 1 : 0;
}

static int multicmp(const void * const va, const void * const vb)
{
  const agram_dchar * const a = * (const agram_dchar * const *) va;
  const agram_dchar * const b = * (const agram_dchar * const *) vb;

  size_t lena = strlen(a);
  size_t lenb = strlen(b);

  if (lena < lenb)
    return -1;
  if (lena > lenb)
    return 1;

  for (size_t i = 0; i < lena; i++)
    {
      if (a[i] == b[i])
        continue;
      if (a[i] == ' ')
        return 1;
      if (b[i] == ' ')
        return -1;
    }

  for (size_t i = 0; i < lena; i++)
    if (a[i] != b[i])
      return a[i] < b[i] ? -1 : a[i] > b[i] ? 1 : 0;

  return 0;
}

static void prnfree(void * str)
{
  puts(str);
  free(str);
}

static int multi(int argc, char * argv[])
{
  if (argc == 3)
    {
      struct vector vec;
      vector_init(&vec);
      const int error = anagrams(argv[2], strlen(argv[2]), append, &vec);
      if (! error)
        {
          vector_sort(&vec, multicmp);
          vector_traverse(&vec, prnfree);
        }
      vector_destroy(&vec);
      return error;
    }
  return usage(argv[0]);
}

static int random(int argc, char * argv[])
{
  const long int count = argc >= 3 ? atol(argv[2]) : 1;
  struct timespec tv;
  clock_gettime(CLOCK_REALTIME, &tv);
  srand(tv.tv_sec ^ tv.tv_nsec);
  for (long int i = 0; i < count; i++)
    {
      const int n = rand() % NWORDS;
      if (prn(words_counts[n].str + strbase, words_counts[n].len, &stdoutfd))
        return 1;
    }
  return 0;
}

static int contained(int argc, char * argv[])
{
  if (argc >= 3)
    return words_from(argv[2], strlen(argv[2]), argc >= 4, prn, &stdoutfd);
  return usage(argv[0]);
}

int main(int argc, char * argv[])
{
  if (init_wl())
    return 1;
  if (argc < 2)
    return usage(argv[0]);

  switch(argv[1][0])
    {
      case 's':
        return single(argc, argv);
      case 'm':
        return multi(argc, argv);
      case 'r':
        return random(argc, argv);
      case 'c':
        return contained(argc, argv);
      default:
        return usage(argv[0]);
    }
}
