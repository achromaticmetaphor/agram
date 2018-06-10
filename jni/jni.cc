#include <cstddef>
#include <memory>

#include <jni.h>

#include "agram_wc.h"
#include "anagram.h"
#include "anagrams.h"
#include "string_view.h"
#include "wordlist.h"
#include "wordlist_source.h"
#include "words_from.h"

using marshalled_ptr = jbyteArray;

extern "C" JNIEXPORT jbyteArray JNICALL
Java_us_achromaticmetaphor_agram_Wordlist_platform(JNIEnv * const env,
                                                   jclass const cl)
{
  static const jbyte csizes[] = PLATFORM;
  jbyteArray sizes = env->NewByteArray(sizeof(csizes) / sizeof(*csizes));
  if (sizes)
    env->SetByteArrayRegion(sizes, 0, sizeof(csizes) / sizeof(*csizes),
                            csizes);
  return sizes;
}

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

jstring new_jstring(JNIEnv * const env, string_view<jchar> sv)
{
  return env->NewString(sv.data, static_cast<jsize>(sv.len));
}

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

class al
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

  al(JNIEnv * const env, jobject const instance)
      : env(env), alinstance(instance),
        aladd(env->GetMethodID(env->GetObjectClass(instance), "add",
                               "(Ljava/lang/Object;)Z"))
  {
  }
};

template <typename C>
static jobject generate(JNIEnv * const env, jstring const string,
                        marshalled_ptr const handle, jobject const alist,
                        C gen)
{
  wordlist const * const wl =
      get_marshalled_pointer<wordlist const>(env, handle);
  al al(env, alist);
  if (!wl || !al)
    return nullptr;

  borrowed_jstring_chars<jchar> str(env, string);
  if (str)
    gen(*wl, str, al);
  return al;
}

extern "C" JNIEXPORT jobject JNICALL
Java_us_achromaticmetaphor_agram_Anagram_generate(JNIEnv * const env,
                                                  jclass const cls,
                                                  const jstring string,
                                                  marshalled_ptr const handle,
                                                  jobject const alist)
{
  return generate(env, string, handle, alist, anagram<al>);
}

extern "C" JNIEXPORT jobject JNICALL
Java_us_achromaticmetaphor_agram_WordsFrom_generate(
    JNIEnv * const env, jclass const cls, jstring const string,
    marshalled_ptr const handle, jboolean const max, jobject const alist)
{
  return generate(env, string, handle, alist,
                  [max](wordlist const & a, string_view<jchar> b, al & d) {
                    return words_from(a, b, max, d);
                  });
}

extern "C" JNIEXPORT jstring JNICALL
Java_us_achromaticmetaphor_agram_Word_pick(JNIEnv * const env,
                                           const jclass cl,
                                           marshalled_ptr const handle,
                                           const jint n)
{
  wordlist const * wl = get_marshalled_pointer<wordlist const>(env, handle);
  return wl ? new_jstring(env, wl->display_string(n)) : nullptr;
}

extern "C" JNIEXPORT jint JNICALL
Java_us_achromaticmetaphor_agram_Wordlist_get_1nwords(
    JNIEnv * const env, jclass const cls, marshalled_ptr const handle)
{
  wordlist const * wl = get_marshalled_pointer<wordlist const>(env, handle);
  if (!wl)
    return 0;
  return wl->words_counts.size();
}

class wr : public wordlist_source
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
  wr(JNIEnv *, jobject);
};

int wr::has_next()
{
  curchars = {};
  return !!(wrcur = local_ref<jstring>(
                env, reinterpret_cast<jstring>(
                         env->CallObjectMethod(wrinstance, wrread))));
}

size_t wr::len() { return env->GetStringLength(wrcur); }

agram_display_char const * wr::get()
{
  return curchars = borrowed_jstring_chars<jchar>(env, wrcur);
}

wr::wr(JNIEnv * const env, jobject const jwords)
    : env(env), wrinstance(jwords), wrcur(), curchars()
{
  auto const wlreader =
      env->FindClass("us/achromaticmetaphor/agram/WordlistReader");
  wrread = wlreader
               ? env->GetMethodID(wlreader, "read", "()Ljava/lang/String;")
               : nullptr;
}

extern "C" JNIEXPORT marshalled_ptr JNICALL
Java_us_achromaticmetaphor_agram_Wordlist_loadNullWordlist(JNIEnv * const env,
                                                           jobject const obj)
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
    JNIEnv * const env, jclass const cls, jstring const jfn,
    jobject const jwords)
{
  wr reader(env, jwords);
  borrowed_jstring_chars<char> fn(env, jfn);
  bool const compile_failed = !fn || reader.compile_wl(fn);
  return compile_failed ? nullptr : load_wl(env, fn);
}

extern "C" JNIEXPORT marshalled_ptr JNICALL
Java_us_achromaticmetaphor_agram_Wordlist_init__Ljava_lang_String_2(
    JNIEnv * const env, jclass const cls, jstring const jfn)
{
  borrowed_jstring_chars<char> fn(env, jfn);
  return fn ? load_wl(env, fn) : nullptr;
}

extern "C" JNIEXPORT void JNICALL
Java_us_achromaticmetaphor_agram_Anagrams_uninit(JNIEnv * const env,
                                                 jclass const cls,
                                                 marshalled_ptr const handle)
{
  delete get_marshalled_pointer<agsto>(env, handle);
}

extern "C" JNIEXPORT marshalled_ptr JNICALL
Java_us_achromaticmetaphor_agram_Anagrams_initState(
    JNIEnv * const env, jclass const cls, jstring const jstr,
    marshalled_ptr const handle)
{
  wordlist const * wl = get_marshalled_pointer<wordlist const>(env, handle);
  borrowed_jstring_chars<jchar> str(env, jstr);
  if (!wl || !str)
    return nullptr;

  return marshalled_pointer(env, std::make_unique<agsto>(*wl, str));
}

extern "C" JNIEXPORT jobject JNICALL
Java_us_achromaticmetaphor_agram_Anagrams_generate(
    JNIEnv * const env, jclass const cls, jint const n, jobject const alist,
    marshalled_ptr const handle)
{
  al al(env, alist);
  agsto * const state = get_marshalled_pointer<agsto>(env, handle);
  if (!al || !state)
    return nullptr;

  jint i;
  for (i = 0; i < n; i++)
    {
      auto const next = state->single();
      if (!next)
        break;
      if (al.add(*next))
        break;
    }
  return al;
}
