#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <jni.h>

#include "local_ref.h"
#include "string_view.h"

jstring new_jstring(JNIEnv * const env, string_view<jchar> sv)
{
  return env->NewString(sv.data, static_cast<jsize>(sv.len));
}

class array_list
{
  JNIEnv * env;
  jmethodID aladd;
  jobject alinstance;

public:
  int add(string_view<jchar> sv)
  {
    local_ref<jstring> s(env, new_jstring(env, sv));
    if (!s)
      return 1;
    env->CallBooleanMethod(alinstance, aladd, jstring(s));
    return 0;
  }

  int operator()(string_view<jchar> sv) { return add(sv); }

  explicit operator bool() const { return !!aladd; }
  operator jobject() const { return alinstance; }

  array_list(JNIEnv * const env, jobject const instance)
      : env(env), alinstance(instance),
        aladd(env->GetMethodID(env->GetObjectClass(instance), "add",
                               "(Ljava/lang/Object;)Z"))
  {
  }
};

#endif
