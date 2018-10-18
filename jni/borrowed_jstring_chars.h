#ifndef BORROWED_JSTRING_CHARS_H
#define BORROWED_JSTRING_CHARS_H

#include <cstddef>

#include <jni.h>

#include "string_view.h"

template <typename C>
static C const * borrowChars(JNIEnv *, jstring) = delete;
template <typename C>
static void releaseChars(JNIEnv *, jstring, C const *) = delete;

template <>
char const * borrowChars<char>(JNIEnv * const env, jstring const jstr)
{
  return env->GetStringUTFChars(jstr, 0);
}

template <>
void releaseChars<char>(JNIEnv * const env, jstring const jstr,
                        char const * str)
{
  env->ReleaseStringUTFChars(jstr, str);
}

template <>
jchar const * borrowChars<jchar>(JNIEnv * const env, jstring const jstr)
{
  return env->GetStringChars(jstr, 0);
}

template <>
void releaseChars<jchar>(JNIEnv * const env, jstring const jstr,
                         jchar const * str)
{
  env->ReleaseStringChars(jstr, str);
}

template <typename C> class borrowed_jstring_chars
{
  JNIEnv * env;
  jstring jstr;
  C const * str;

public:
  explicit operator bool() const { return str != nullptr; }
  operator C const *() const { return str; }
  operator string_view<C>() const
  {
    return {str, static_cast<std::size_t>(size())};
  }

  jsize size() const { return env->GetStringLength(jstr); }

  borrowed_jstring_chars<C> &
  operator=(borrowed_jstring_chars<C> const &) = delete;

  borrowed_jstring_chars<C> & operator=(borrowed_jstring_chars<C> && k)
  {
    env = k.env;
    jstr = k.jstr;
    str = k.str;
    k.str = nullptr;
    return *this;
  }

  borrowed_jstring_chars() : env(nullptr), jstr(nullptr), str(nullptr) {}
  borrowed_jstring_chars(borrowed_jstring_chars<C> const &) = delete;

  borrowed_jstring_chars(borrowed_jstring_chars<C> && k)
      : env(k.env), jstr(k.jstr), str(k.str)
  {
    k.str = nullptr;
  }

  borrowed_jstring_chars(JNIEnv * const env, jstring const jstr)
      : env(env), jstr(jstr), str(borrowChars<C>(env, jstr))
  {
  }

  ~borrowed_jstring_chars()
  {
    if (str)
      releaseChars<C>(env, jstr, str);
  }
};

#endif
