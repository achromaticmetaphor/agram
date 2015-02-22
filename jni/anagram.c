#include <stddef.h>
#include <stdlib.h>

#include "agram_wc.h"
#include "is_anagram.h"
#include "lcwc.h"

#include <jni.h>
#include "jnihelp.h"

static size_t anagram_generate(const jint * const str, const jint * out[])
{
  size_t i;
  size_t j = 0;
  struct wc target;
  if (wc_init(&target, str))
    return 0;
  for (i = 0; i < NWORDS; i++)
    if (is_anagram(&target, words_counts+i))
      out[j++] = strbase + words_counts[i].str;
  wc_free(&target);
  return j;
}

JNIEXPORT jobjectArray JNICALL Java_us_achromaticmetaphor_agram_Anagram_generate_1native
  (JNIEnv * const env, const jclass class, const jintArray string)
{
  const jint * * const tmp = malloc(sizeof(*tmp) * NWORDS);
  jint * const str = tmp ? (*env)->GetIntArrayElements(env, string, NULL) : NULL;
  const size_t len = str ? anagram_generate(str, tmp) : 0;
  if (str)
    (*env)->ReleaseIntArrayElements(env, string, str, JNI_ABORT);
  const jobjectArray array = arr2jarr(env, tmp, len);
  free(tmp);
  return array;
}
