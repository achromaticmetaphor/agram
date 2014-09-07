#include "agram_wc.h"

#include <jni.h>

JNIEXPORT jstring JNICALL Java_us_achromaticmetaphor_agram_Word_pick_1native
  (JNIEnv * env, jclass class, jint n)
{
  return (*env)->NewStringUTF(env, words_counts[n].str);
}

JNIEXPORT jint JNICALL Java_us_achromaticmetaphor_agram_Word_get_1nwords
  (JNIEnv * env, jclass class)
{
  return NWORDS;
}
