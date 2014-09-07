#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "lcwc.h"
#include "agram_wc.h"
#include "is_anagram.h"
#include "is_within.h"
#include "mlen.h"

#include <jni.h>
#include "jnihelp.h"

MLEN_DECL(wc_p, const struct wc *)

static const struct wc * * filter_wc(const struct wc * const * wcs, const struct wc * target, int (* ff)(const struct wc *, const struct wc *))
{
  const struct wc * * out = malloc(sizeof(*out) * (mlen_wc_p(wcs)+1));
  if (out)
    {
      const struct wc * * outp = out;
      for (; *wcs; wcs++)
        if(ff(*wcs, target))
          *outp++ = *wcs;
      *outp = NULL;
    }
  return out;
}

static const struct wc * * alift(const struct wc * const wcs, const size_t len)
{
  size_t i;
  const struct wc * * out = malloc(sizeof(*out) * (len+1));
  if (out)
    {
      for (i = 0; i < len; i++)
        out[i] = wcs+i;
      out[len] = NULL;
    }
  return out;
}

static size_t anagrams_print(const struct wc * const target, char * const prefix, const size_t offset, const struct wc * const * const wcs_in, const int fast, const char * * out, size_t n)
{
  prefix[offset] = ' ';
  const struct wc * * const wcs = filter_wc(wcs_in, target, is_within);
  if (! wcs)
    return n;
  const struct wc * const * wcsp;
  for (wcsp = wcs; *wcsp; wcsp++)
    {
      strcpy(prefix+offset+1, (**wcsp).str);
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
          n = anagrams_print(&new_target, prefix, offset + strlen((**wcsp).str) + 1, fast ? wcsp : wcs, fast, out, n);
          wc_free(&new_target);
        }
    }
  free(wcs);
  return n;
}

static size_t anagrams_generate(const char * s, const char * out[], int fast)
{
  struct wc target;
  const struct wc * * const wcs = alift(words_counts, NWORDS);
  char * const prefix = wcs ? malloc(sizeof(*prefix) * (strlen(s)*2+1)) : NULL;
  if (wc_init(&target, s))
    return 0;
  size_t n = prefix ? anagrams_print(&target, prefix, 0, wcs, fast, out, 0) : 0;
  free(wcs);
  free(prefix);
  wc_free(&target);
  return n;
}

static void ag_free(const char * * in, size_t n)
{
  while (n--)
    free((void *) in[n]);
}

JNIEXPORT jobjectArray JNICALL Java_us_achromaticmetaphor_agram_Anagrams_generate_1native
  (JNIEnv * env, jclass class, jstring str, jboolean full)
{
  size_t i;
  const char * s = (*env)->GetStringUTFChars(env, str, NULL);
  const char * * tmp = s ? malloc(sizeof(*tmp) * NWORDS * strlen(s)) : NULL;
  const size_t len = tmp ? anagrams_generate(s, tmp, !full) : 0;
  if (s)
    (*env)->ReleaseStringUTFChars(env, str, s);
  jobjectArray array = arr2jarr(env, tmp, len);
  ag_free(tmp, len);
  free(tmp);
  return array;
}
