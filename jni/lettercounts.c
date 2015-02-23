#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "astr.h"

#include <jni.h>

static void insert(const jint c, jint l[], size_t * const n)
{
  size_t i;
  for (i = 0; i < *n; i++)
    if (c == l[i])
      return;
  l[(*n)++] = c;
}

static int wccmp(const void * const va, const void * const vb)
{
  const jint * const a = va;
  const jint * const b = vb;
  return *a < *b ? -1 : *a != *b;
}

static size_t lettercounts_s(unsigned int counts[], jint letters[], const jint * const str)
{
  size_t i;
  const jint * s;
  const size_t len = astrlen(str);
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

size_t lettercounts(unsigned int counts[], jint letters[], const jchar * const str, const jsize slen)
{
  jint tmp[slen+1];
  uastrcpy(tmp, str, slen);
  return lettercounts_s(counts, letters, tmp);
}
