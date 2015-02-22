#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "astr.h"

#include <jni.h>

jobjectArray arr2jarr(JNIEnv * const env, const jint * const sarr[], const size_t len)
{
  size_t i;
  jclass const iarr = (*env)->FindClass(env, "[I");
  if (! iarr)
    return NULL;
  const jobjectArray array = (*env)->NewObjectArray(env, len, iarr, NULL);
  if (array)
    for (i = 0; i < len; i++)
      {
        const size_t slen = astrlen(sarr[i]) + 1;
        const jintArray s = (*env)->NewIntArray(env, slen);
        if (! s)
          return arr2jarr(env, NULL, 0);
        (*env)->SetIntArrayRegion(env, s, 0, slen, sarr[i]);
        (*env)->SetObjectArrayElement(env, array, i, s);
        (*env)->DeleteLocalRef(env, s);
      }
  return array;
}

const jint * * jarr2arr(JNIEnv * const env, jobjectArray const jarr, size_t * const lenout)
{
  *lenout = 0;
  jsize const len = (*env)->GetArrayLength(env, jarr);
  const jint * * const arr = malloc(sizeof(*arr) * len);
  if (! arr)
    return NULL;

  jsize i;
  for (i = 0; i < len; i++)
    {
      jintArray const s = (*env)->GetObjectArrayElement(env, jarr, i);
      arr[i] = (*env)->GetIntArrayElements(env, s, NULL);
      (*env)->DeleteLocalRef(env, s);
    }

  *lenout = len;
  return arr;
}

void free_jarr(JNIEnv * const env, jobjectArray const jarr, jint * * const arr)
{
  jsize const len = (*env)->GetArrayLength(env, jarr);
  jsize i;
  for (i = 0; i < len; i++)
    {
      jintArray const s = (*env)->GetObjectArrayElement(env, jarr, i);
      (*env)->ReleaseIntArrayElements(env, s, arr[i], JNI_ABORT);
      (*env)->DeleteLocalRef(env, s);
    }

  free(arr);
}
