#include <jni.h>

#include "agram_wc.h"
#include "is_anagram.h"
#include "lcwc.h"

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_Anagram_generate
  (JNIEnv * const env, const jobject obj, const jstring string)
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
      for (i = 0; i < NWORDS; i++)
        if (is_anagram(&target, words_counts+i))
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
