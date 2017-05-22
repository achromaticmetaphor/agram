#define _POSIX_C_SOURCE 200809L

#include <cstddef>
#include <cstdlib>
#include <cstring>

#include "agram_types.h"

#if AGRAM_ANDROID
#include "astr.h"
#endif

static void insert(const agram_cpt c, agram_cpt l[], size_t * const n)
{
  size_t i;
  for (i = 0; i < *n; i++)
    if (c == l[i])
      return;
  l[(*n)++] = c;
}

static int wccmp(const void * const va, const void * const vb)
{
  const agram_cpt * const a = (typeof(a)) va;
  const agram_cpt * const b = (typeof(b)) vb;
  return *a < *b ? -1 : *a != *b;
}

static size_t lettercounts_s(unsigned int counts[], agram_cpt letters[], const agram_cpt * const str, const size_t len)
{
  size_t i;
  const agram_cpt * s;
  size_t nmemb = 0;

  for (s = str; *s; s++)
    insert(*s, letters, &nmemb);
  qsort(letters, nmemb, sizeof(*letters), wccmp);

  for (i = 0; i < nmemb; i++)
    {
      size_t j;
      counts[i] = 0;
      for (j = 0; j < len; j++)
        if (letters[i] == str[j])
          counts[i]++;
    }

  return nmemb;
}

size_t lettercounts(unsigned int counts[], agram_cpt letters[], const agram_dchar * const str, const size_t slen)
{
#if AGRAM_ANDROID
  agram_cpt tmp[slen+1];
  return lettercounts_s(counts, letters, tmp, uastrcpy(tmp, str, slen));
#else
  const size_t wlen = mbstowcs(NULL, str, 0);
  agram_cpt tmp[wlen+1];
  mbstowcs(tmp, str, wlen+1);
  return lettercounts_s(counts, letters, tmp, wlen);
#endif
}
