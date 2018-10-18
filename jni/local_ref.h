#ifndef LOCAL_REF_H
#define LOCAL_REF_H

#include <jni.h>

template <typename T> class local_ref
{
  JNIEnv * env;
  T ref;

public:
  explicit operator bool() const { return ref != nullptr; }
  operator T() const { return ref; }

  local_ref() : env(nullptr), ref(nullptr) {}
  local_ref(JNIEnv * const env, T const ref) : env(env), ref(ref) {}

  local_ref<T> & operator=(local_ref<T> && r)
  {
    env = r.env;
    ref = r.ref;
    r.ref = nullptr;
    return *this;
  }

  ~local_ref()
  {
    if (ref)
      env->DeleteLocalRef(ref);
  }
};

#endif
