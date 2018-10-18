#include <cstddef>
#include <memory>

#include <jni.h>

#include "anagram.h"
#include "anagrams.h"
#include "array_list.h"
#include "borrowed_jstring_chars.h"
#include "marshalled_pointer.h"
#include "string_view.h"
#include "wordlist.h"
#include "wordlist_reader.h"
#include "words_from.h"

extern "C" JNIEXPORT jbyteArray JNICALL
Java_us_achromaticmetaphor_agram_Wordlist_platform(JNIEnv * env, jclass)

{
  static const jbyte csizes[] = PLATFORM;
  jbyteArray sizes = env->NewByteArray(sizeof(csizes) / sizeof(*csizes));
  if (sizes)
    env->SetByteArrayRegion(sizes, 0, sizeof(csizes) / sizeof(*csizes),
                            csizes);
  return sizes;
}

template <typename C>
static jobject generate(JNIEnv * const env, jstring const string,
                        marshalled_ptr const handle, jobject const alist,
                        C gen)
{
  wordlist const * const wl =
      get_marshalled_pointer<wordlist const>(env, handle);
  array_list al(env, alist);
  if (!wl || !al)
    return nullptr;

  borrowed_jstring_chars<jchar> str(env, string);
  if (str)
    gen(*wl, str, al);
  return al;
}

extern "C" JNIEXPORT jobject JNICALL
Java_us_achromaticmetaphor_agram_Anagram_generate(JNIEnv * env, jclass,
                                                  const jstring string,
                                                  marshalled_ptr const handle,
                                                  jobject const alist)
{
  return generate(env, string, handle, alist, anagram<array_list>);
}

extern "C" JNIEXPORT jobject JNICALL
Java_us_achromaticmetaphor_agram_WordsFrom_generate(
    JNIEnv * env, jclass, jstring const string, marshalled_ptr const handle,
    jboolean const max, jobject const alist)
{
  return generate(env, string, handle, alist,
                  [max](wordlist const & a, string_view<jchar> b,
                        array_list & d) { return words_from(a, b, max, d); });
}

extern "C" JNIEXPORT jstring JNICALL
Java_us_achromaticmetaphor_agram_Word_pick(JNIEnv * env, jclass,
                                           marshalled_ptr const handle,
                                           const jint n)
{
  wordlist const * wl = get_marshalled_pointer<wordlist const>(env, handle);
  return wl ? new_jstring(env, wl->display_string(n)) : nullptr;
}

extern "C" JNIEXPORT jint JNICALL
Java_us_achromaticmetaphor_agram_Wordlist_get_1nwords(
    JNIEnv * env, jclass, marshalled_ptr const handle)
{
  wordlist const * wl = get_marshalled_pointer<wordlist const>(env, handle);
  if (!wl)
    return 0;
  return wl->words_counts.size();
}

extern "C" JNIEXPORT marshalled_ptr JNICALL
Java_us_achromaticmetaphor_agram_Wordlist_loadNullWordlist(JNIEnv * env,
                                                           jobject)
{
  return marshalled_pointer(env, std::make_unique<wordlist>());
}

static marshalled_ptr load_wl(JNIEnv * const env, const char * const fn)
{
  auto wl = std::make_unique<wordlist>();
  if (load_wl(wl.get(), fn))
    return nullptr;
  else
    return marshalled_pointer(env, std::move(wl));
}

extern "C" JNIEXPORT marshalled_ptr JNICALL
Java_us_achromaticmetaphor_agram_Wordlist_init__Ljava_lang_String_2Lus_achromaticmetaphor_agram_WordlistReader_2(
    JNIEnv * env, jclass, jstring const jfn, jobject const jwords)
{
  wordlist_reader reader(env, jwords);
  borrowed_jstring_chars<char> fn(env, jfn);
  bool const compile_failed = !fn || reader.compile_wl(fn);
  return compile_failed ? nullptr : load_wl(env, fn);
}

extern "C" JNIEXPORT marshalled_ptr JNICALL
Java_us_achromaticmetaphor_agram_Wordlist_init__Ljava_lang_String_2(
    JNIEnv * env, jclass, jstring const jfilepath)
{
  borrowed_jstring_chars<char> filepath(env, jfilepath);
  return filepath ? load_wl(env, filepath) : nullptr;
}

extern "C" JNIEXPORT void JNICALL
Java_us_achromaticmetaphor_agram_Anagrams_uninit(JNIEnv * env, jclass,
                                                 marshalled_ptr const handle)
{
  delete get_marshalled_pointer<agsto>(env, handle);
}

extern "C" JNIEXPORT marshalled_ptr JNICALL
Java_us_achromaticmetaphor_agram_Anagrams_initState(
    JNIEnv * env, jclass, jstring const jstr, marshalled_ptr const handle)
{
  wordlist const * wl = get_marshalled_pointer<wordlist const>(env, handle);
  borrowed_jstring_chars<jchar> str(env, jstr);
  if (!wl || !str)
    return nullptr;

  return marshalled_pointer(env, std::make_unique<agsto>(*wl, str));
}

extern "C" JNIEXPORT jobject JNICALL
Java_us_achromaticmetaphor_agram_Anagrams_generate(
    JNIEnv * env, jclass, jint const n, jobject const alist,
    marshalled_ptr const handle)
{
  array_list al(env, alist);
  agsto * const state = get_marshalled_pointer<agsto>(env, handle);
  if (!al || !state)
    return nullptr;

  for (jint i = 0; i < n; ++i)
    {
      auto const next = state->single();
      if (!next || al.add(*next))
        break;
    }
  return al;
}
