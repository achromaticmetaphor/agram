#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "agram_wc.h"
#include "astr.h"
#include "is_anagram.h"
#include "is_within.h"
#include "lcwc.h"

#include <jni.h>
#include "jnihelp.h"

static size_t filter_lc(const struct lc * * const out, const struct lc * const * wcs, const struct wc * const target)
{
  const struct lc * * outp = out;
  for (; *wcs; wcs++)
    if(is_within_lw(*wcs, target))
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

static size_t anagrams_print(const struct wc * const target, jint * const prefix, const size_t offset, const struct lc * const * const wcs_in, const int fast, const jint * out[], const struct lc * * const wcs)
{
  prefix[offset] = 32;
  const size_t wcslen = filter_lc(wcs, wcs_in, target);
  size_t n = 0;
  const struct lc * const * wcsp;
  for (wcsp = wcs; *wcsp; wcsp++)
    {
      astrcpy(prefix+offset+1, strbase + (**wcsp).str);
      if (is_anagram(target, *wcsp))
        {
          if (out[n] = astrdup(prefix+1))
            n++;
          else
            return n;
        }
      else
        {
          struct wc new_target;
          if (wc_sub(&new_target, target, *wcsp))
            return n;
          n += anagrams_print(&new_target, prefix, offset + (**wcsp).len + 1, fast ? wcsp : wcs, fast, out + n, wcs + wcslen + 1);
          wc_free(&new_target);
        }
    }
  return n;
}

static size_t anagrams_generate(const jint * const s, const jint * out[], const int fast)
{
  struct wc target;
  if (wc_init(&target, s))
    return 0;
  const struct lc * * const wcs = alift(words_counts, NWORDS);
  const size_t prefsize = astrlen(s) * 2 + 1;
  jint * const prefix = wcs ? malloc(sizeof(*prefix) * prefsize) : NULL;
  const struct lc * * const scratch = prefix ? malloc(sizeof(*scratch) * (NWORDS + 1) * prefsize) : NULL;
  const size_t n = scratch ? anagrams_print(&target, prefix, 0, wcs, fast, out, scratch) : 0;
  free(scratch);
  free(wcs);
  free(prefix);
  wc_free(&target);
  return n;
}

static void ag_free(const jint * in[], size_t n)
{
  while (n--)
    free((void *) in[n]);
}

JNIEXPORT jobjectArray JNICALL Java_us_achromaticmetaphor_agram_Anagrams_generate_1native
  (JNIEnv * const env, const jclass class, const jintArray str, const jboolean full)
{
  size_t i;
  jint * const s = (*env)->GetIntArrayElements(env, str, NULL);
  const jint * * const tmp = s ? malloc(sizeof(*tmp) * NWORDS * astrlen(s)) : NULL;
  const size_t len = tmp ? anagrams_generate(s, tmp, !full) : 0;
  if (s)
    (*env)->ReleaseIntArrayElements(env, str, s, JNI_ABORT);
  const jobjectArray array = arr2jarr(env, tmp, len);
  ag_free(tmp, len);
  free(tmp);
  return array;
}
