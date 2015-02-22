#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#include "agram_wc.h"
#include "is_within.h"
#include "lcwc.h"

#include <jni.h>
#include "jnihelp.h"

static void wc_max(const struct wc * const lets)
{
  unsigned int i;
  for (i = 0; i < lets->nchars; i++)
    lets->counts[i] = UINT_MAX;
}

static size_t words_from_generate(const jint * const str, const jint * out[], const int max)
{
  size_t i, j = 0;
  struct wc target;
  if (wc_init(&target, str))
    return 0;

  if (max)
    wc_max(&target);

  for (i = 0; i < NWORDS; i++)
    if (is_within_lw(words_counts+i, &target))
      out[j++] = strbase + words_counts[i].str;

  wc_free(&target);
  return j;
}

JNIEXPORT jobjectArray JNICALL Java_us_achromaticmetaphor_agram_WordsFrom_generate_1native
  (JNIEnv * const env, const jclass class, const jintArray string, const jboolean max)
{
  const jint * * const tmp = malloc(sizeof(*tmp) * NWORDS);
  jint * const str = tmp ? (*env)->GetIntArrayElements(env, string, NULL) : NULL;
  const size_t len = str ? words_from_generate(str, tmp, max) : 0;
  if (str)
    (*env)->ReleaseIntArrayElements(env, string, str, JNI_ABORT);
  const jobjectArray array = arr2jarr(env, tmp, len);
  free(tmp);
  return array;
}
