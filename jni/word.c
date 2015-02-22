#include "agram_wc.h"

#include <jni.h>

JNIEXPORT jintArray JNICALL Java_us_achromaticmetaphor_agram_Word_pick_1native
  (JNIEnv * const env, const jclass class, const jint n)
{
  jintArray const s = (*env)->NewIntArray(env, words_counts[n].len + 1);
  if (s)
    (*env)->SetIntArrayRegion(env, s, 0, words_counts[n].len + 1, words_counts[n].str + strbase);
  return s;
}

JNIEXPORT jint JNICALL Java_us_achromaticmetaphor_agram_Word_get_1nwords
  (JNIEnv * const env, const jclass class)
{
  return NWORDS;
}
