#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "lcwc.h"
#include "agram_wc.h"
#include "is_anagram.h"
#include "is_within.h"

#include <jni.h>
#include "jnihelp.h"

static size_t filter_lc(const struct lc * * const out, const struct lc * const * wcs, const struct wc * const target, int (* const ff)(const struct lc *, const struct wc *))
{
  const struct lc * * outp = out;
  for (; *wcs; wcs++)
    if(ff(*wcs, target))
      *outp++ = *wcs;
  *outp = NULL;
  return outp - out;
}

static const struct lc * * alift(const struct lc * const wcs, const size_t len)
{
  size_t i;
  const struct lc * * const out = malloc(sizeof(*out) * (len+1));
  if (out)
    {
      for (i = 0; i < len; i++)
        out[i] = wcs+i;
      out[len] = NULL;
    }
  return out;
}

static size_t anagrams_print(const struct wc * const target, char * const prefix, const size_t offset, const struct lc * const * const wcs_in, const size_t wcs_in_len, const int fast, const char * out[], size_t n)
{
  prefix[offset] = ' ';
  const struct lc * * const wcs = malloc(sizeof(*wcs) * (wcs_in_len + 1));
  if (! wcs)
    return n;
  const size_t wcslen = filter_lc(wcs, wcs_in, target, is_within_lw);
  const struct lc * const * wcsp;
  for (wcsp = wcs; *wcsp; wcsp++)
    {
      strcpy(prefix+offset+1, strbase + (**wcsp).str);
      if (is_anagram(target, *wcsp))
        {
          if (out[n] = strdup(prefix+1))
            n++;
          else
            return n;
        }
      else
        {
          struct wc new_target;
          if (wc_sub(&new_target, target, *wcsp))
            return n;
          n = anagrams_print(&new_target, prefix, offset + strlen(strbase + (**wcsp).str) + 1, fast ? wcsp : wcs, fast ? wcslen - (wcsp - wcs) : wcslen, fast, out, n);
          wc_free(&new_target);
        }
    }
  free(wcs);
  return n;
}

static size_t anagrams_generate(const char * const s, const char * out[], const int fast)
{
  struct wc target;
  const struct lc * * const wcs = alift(words_counts, NWORDS);
  char * const prefix = wcs ? malloc(sizeof(*prefix) * (strlen(s)*2+1)) : NULL;
  if (wc_init(&target, s))
    return 0;
  size_t n = prefix ? anagrams_print(&target, prefix, 0, wcs, NWORDS, fast, out, 0) : 0;
  free(wcs);
  free(prefix);
  wc_free(&target);
  return n;
}

static void ag_free(const char * in[], size_t n)
{
  while (n--)
    free((void *) in[n]);
}

JNIEXPORT jobjectArray JNICALL Java_us_achromaticmetaphor_agram_Anagrams_generate_1native
  (JNIEnv * const env, const jclass class, const jstring str, const jboolean full)
{
  size_t i;
  const char * const s = (*env)->GetStringUTFChars(env, str, NULL);
  const char * * const tmp = s ? malloc(sizeof(*tmp) * NWORDS * strlen(s)) : NULL;
  const size_t len = tmp ? anagrams_generate(s, tmp, !full) : 0;
  if (s)
    (*env)->ReleaseStringUTFChars(env, str, s);
  const jobjectArray array = arr2jarr(env, tmp, len);
  ag_free(tmp, len);
  free(tmp);
  return array;
}
