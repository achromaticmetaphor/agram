#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "agram_wc.h"
#include "astr.h"
#include "is_anagram.h"
#include "is_within.h"
#include "lcwc.h"

#include <jni.h>

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

static void anagrams_print(JNIEnv * const env, const struct wc * const target, jchar * const prefix, const size_t offset, const struct lc * const * const wcs_in, const jobject list, const jmethodID aladd, const struct lc * * const wcs)
{
  prefix[offset] = 32;
  const size_t wcslen = filter_lc(wcs, wcs_in, target);
  const struct lc * const * wcsp;
  for (wcsp = wcs; *wcsp; wcsp++)
    {
      memcpy(prefix+offset+1, strbase + (**wcsp).str, sizeof(*prefix) * (**wcsp).len);
      if (is_anagram(target, *wcsp))
        {
          jstring s = (*env)->NewString(env, prefix + 1, offset + (**wcsp).len);
          if (! s)
            return;
          (*env)->CallBooleanMethod(env, list, aladd, s);
          (*env)->DeleteLocalRef(env, s);
        }
      else
        {
          struct wc new_target;
          if (wc_sub(&new_target, target, *wcsp))
            return;
          anagrams_print(env, &new_target, prefix, offset + (**wcsp).len + 1, wcsp, list, aladd, wcs + wcslen + 1);
          wc_free(&new_target);
        }
    }
}

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_Anagrams_generate_1native
  (JNIEnv * const env, const jclass class, const jstring string)
{
  jclass arraylist = (*env)->FindClass(env, "java/util/ArrayList");
  jmethodID alcon = arraylist ? (*env)->GetMethodID(env, arraylist, "<init>", "()V") : NULL;
  jmethodID aladd = alcon ? (*env)->GetMethodID(env, arraylist, "add", "(Ljava/lang/Object;)Z") : NULL;
  jobject list = aladd ? (*env)->NewObject(env, arraylist, alcon) : NULL;
  jchar const * const str = list ? (*env)->GetStringChars(env, string, NULL) : NULL;
  struct wc target;
  if (str && ! wc_init(&target, str, (*env)->GetStringLength(env, string)))
    {
      const size_t prefsize = target.len * 2 + 1;
      const struct lc * * const wcs = alift(words_counts, NWORDS);
      jchar * const prefix = wcs ? malloc(sizeof(*prefix) * prefsize) : NULL;
      const struct lc * * const scratch = prefix ? malloc(sizeof(*scratch) * (NWORDS + 1) * prefsize) : NULL;
      if (scratch)
        anagrams_print(env, &target, prefix, 0, wcs, list, aladd, scratch);
      free(scratch);
      free(wcs);
      free(prefix);
      wc_free(&target);
    }
  if (str)
    (*env)->ReleaseStringChars(env, string, str);
  return list;
}
