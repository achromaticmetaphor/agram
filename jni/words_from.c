#include <jni.h>
#include <limits.h>

#include "agram_wc.h"
#include "is_within.h"
#include "lcwc.h"

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_WordsFrom_generate
  (JNIEnv * const env, const jobject obj, const jstring string, const jboolean max)
{
  jclass arraylist = (*env)->FindClass(env, "java/util/ArrayList");
  jmethodID alcon = arraylist ? (*env)->GetMethodID(env, arraylist, "<init>", "()V") : NULL;
  jmethodID aladd = alcon ? (*env)->GetMethodID(env, arraylist, "add", "(Ljava/lang/Object;)Z") : NULL;
  jobject list = aladd ? (*env)->NewObject(env, arraylist, alcon) : NULL;
  jchar const * const str = list ? (*env)->GetStringChars(env, string, NULL) : NULL;
  struct wc target;
  if (str && ! wc_init(&target, str, (*env)->GetStringLength(env, string)))
    {
      jsize i;
      if (max)
        for (i = 0; i < target.nchars; i++)
          target.counts[i] = UINT_MAX;
      for (i = 0; i < NWORDS; i++)
        if (is_within_lw(words_counts+i, &target))
          {
            jstring s = (*env)->NewString(env, words_counts[i].str + strbase, words_counts[i].len);
            if (! s)
              break;
            (*env)->CallBooleanMethod(env, list, aladd, s);
            (*env)->DeleteLocalRef(env, s);
          }
    }
  if (str)
    (*env)->ReleaseStringChars(env, string, str);
  return list;
}
