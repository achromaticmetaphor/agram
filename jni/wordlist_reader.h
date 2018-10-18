#ifndef WORDLIST_READER_H
#define WORDLIST_READER_H

#include <cstddef>

#include <jni.h>

#include "agram_types.h"
#include "borrowed_jstring_chars.h"
#include "local_ref.h"
#include "wordlist_source.h"

class wordlist_reader : public wordlist_source
{
  JNIEnv * env;
  jmethodID wrread;
  jobject wrinstance;
  local_ref<jstring> wrcur;
  borrowed_jstring_chars<jchar> curchars;

public:
  int has_next();
  size_t len();
  agram_display_char const * get();
  wordlist_reader(JNIEnv *, jobject);
};

int wordlist_reader::has_next()
{
  curchars = {};
  return !!(wrcur = local_ref<jstring>(
                env, reinterpret_cast<jstring>(
                         env->CallObjectMethod(wrinstance, wrread))));
}

size_t wordlist_reader::len() { return env->GetStringLength(wrcur); }

agram_display_char const * wordlist_reader::get()
{
  return curchars = borrowed_jstring_chars<jchar>(env, wrcur);
}

wordlist_reader::wordlist_reader(JNIEnv * const env, jobject const jwords)
    : env(env), wrinstance(jwords), wrcur(), curchars()
{
  auto const wlreader =
      env->FindClass("us/achromaticmetaphor/agram/WordlistReader");
  wrread = wlreader
               ? env->GetMethodID(wlreader, "read", "()Ljava/lang/String;")
               : nullptr;
}

#endif
