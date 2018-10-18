#ifndef MARSHALLED_POINTER_H
#define MARSHALLED_POINTER_H

#include <jni.h>

using marshalled_ptr = jbyteArray;

template <typename T>
static T * get_marshalled_pointer(JNIEnv * const env,
                                  marshalled_ptr const arr)
{
  if (!arr)
    return nullptr;

  T * pointer;
  env->GetByteArrayRegion(arr, 0, sizeof(T *) / sizeof(jbyte),
                          (jbyte *) &pointer);
  return pointer;
}

template <typename T>
static marshalled_ptr marshalled_pointer(JNIEnv * const env,
                                         std::unique_ptr<T> && uptr)
{
  jbyteArray const result = env->NewByteArray(sizeof(void *) / sizeof(jbyte));
  if (result)
    {
      T * ptr = uptr.release();
      env->SetByteArrayRegion(result, 0, sizeof(T *) / sizeof(jbyte),
                              reinterpret_cast<jbyte const *>(&ptr));
    }
  return result;
}

#endif
