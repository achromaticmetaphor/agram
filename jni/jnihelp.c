#include <stddef.h>

#include <jni.h>

jobjectArray arr2jarr(JNIEnv * env, const char * * sarr, size_t len)
{
  size_t i;
  jclass string = (*env)->FindClass(env, "java/lang/String");
  if (! string)
    return NULL;
  jobjectArray array = (*env)->NewObjectArray(env, len, string, NULL);
  if (array)
    for (i = 0; i < len; i++)
      {
        jstring s = (*env)->NewStringUTF(env, sarr[i]);
        if (! s)
          return arr2jarr(env, NULL, 0);
        (*env)->SetObjectArrayElement(env, array, i, s);
        (*env)->DeleteLocalRef(env, s);
      }
  return array;
}
