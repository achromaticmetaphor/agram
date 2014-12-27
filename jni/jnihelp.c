#include <stddef.h>
#include <stdlib.h>

#include <jni.h>

jobjectArray arr2jarr(JNIEnv * const env, const char * const sarr[], const size_t len)
{
  size_t i;
  const jclass string = (*env)->FindClass(env, "java/lang/String");
  if (! string)
    return NULL;
  const jobjectArray array = (*env)->NewObjectArray(env, len, string, NULL);
  if (array)
    for (i = 0; i < len; i++)
      {
        const jstring s = (*env)->NewStringUTF(env, sarr[i]);
        if (! s)
          return arr2jarr(env, NULL, 0);
        (*env)->SetObjectArrayElement(env, array, i, s);
        (*env)->DeleteLocalRef(env, s);
      }
  return array;
}

const char * * jarr2arr(JNIEnv * const env, jobjectArray const jarr, size_t * const lenout)
{
  *lenout = 0;
  jsize const len = (*env)->GetArrayLength(env, jarr);
  const char * * const arr = malloc(sizeof(*arr) * len);
  if (! arr)
    return NULL;

  jsize i;
  for (i = 0; i < len; i++)
    {
      jstring const s = (*env)->GetObjectArrayElement(env, jarr, i);
      arr[i] = (*env)->GetStringUTFChars(env, s, NULL);
      (*env)->DeleteLocalRef(env, s);
    }

  *lenout = len;
  return arr;
}

void free_jarr(JNIEnv * const env, jobjectArray const jarr, const char * * const arr)
{
  jsize const len = (*env)->GetArrayLength(env, jarr);
  jsize i;
  for (i = 0; i < len; i++)
    {
      jstring const s = (*env)->GetObjectArrayElement(env, jarr, i);
      (*env)->ReleaseStringUTFChars(env, s, arr[i]);
      (*env)->DeleteLocalRef(env, s);
    }

  free(arr);
}
