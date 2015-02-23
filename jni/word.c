#include "agram_wc.h"

#include <jni.h>

JNIEXPORT jstring JNICALL Java_us_achromaticmetaphor_agram_Word_pick_1native
  (JNIEnv * const env, const jclass class, const jint n)
{
  return (*env)->NewString(env, words_counts[n].str + strbase, words_counts[n].len);
}

JNIEXPORT jint JNICALL Java_us_achromaticmetaphor_agram_Word_get_1nwords
  (JNIEnv * const env, const jclass class)
{
  return NWORDS;
}
