#include <stddef.h>

#include <jni.h>

#include "agram_wc.h"
#include "anagram.h"
#include "anagrams.h"
#include "wc.h"
#include "words_from.h"

JNIEXPORT jbyteArray JNICALL Java_us_achromaticmetaphor_agram_Wordlist_platform
  (JNIEnv * const env, jclass const class)
{
  static const jbyte csizes[] = PLATFORM;
  jbyteArray sizes = (*env)->NewByteArray(env, sizeof(csizes)/sizeof(*csizes));
  if (sizes)
    (*env)->SetByteArrayRegion(env, sizes, 0, sizeof(csizes)/sizeof(*csizes), csizes);
  return sizes;
}

struct al
{
  JNIEnv * env;
  jmethodID aladd;
  jobject alinstance;
};

static int alinit(struct al * const al, JNIEnv * const env)
{
  al->env = env;
  jclass arraylist = (*env)->FindClass(env, "java/util/ArrayList");
  jmethodID alcon = arraylist ? (*env)->GetMethodID(env, arraylist, "<init>", "()V") : NULL;
  al->aladd = alcon ? (*env)->GetMethodID(env, arraylist, "add", "(Ljava/lang/Object;)Z") : NULL;
  al->alinstance = al->aladd ? (*env)->NewObject(env, arraylist, alcon) : NULL;
  return ! al->alinstance;
}

static int aladd(jchar const * str, size_t const len, void * const val)
{
  struct al * const al = val;
  jstring s = (*al->env)->NewString(al->env, str, len);
  if (! s)
    return 1;
  (*al->env)->CallBooleanMethod(al->env, al->alinstance, al->aladd, s);
  (*al->env)->DeleteLocalRef(al->env, s);
  return 0;
}

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_Anagram_generate
  (JNIEnv * const env, const jobject obj, const jstring string)
{
  struct al al;
  jchar const * const str = alinit(&al, env) ? NULL : (*env)->GetStringChars(env, string, NULL);
  if (str)
    {
      anagram(str, (*env)->GetStringLength(env, string), aladd, &al);
      (*env)->ReleaseStringChars(env, string, str);
    }
  return al.alinstance;
}

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_WordsFrom_generate
  (JNIEnv * const env, const jobject obj, const jstring string, const jboolean max)
{
  struct al al;
  jchar const * const str = alinit(&al, env) ? NULL : (*env)->GetStringChars(env, string, NULL);
  if (str)
    {
      words_from(str, (*env)->GetStringLength(env, string), max, aladd, &al);
      (*env)->ReleaseStringChars(env, string, str);
    }
  return al.alinstance;
}

JNIEXPORT jstring JNICALL Java_us_achromaticmetaphor_agram_Word_pick_1native
  (JNIEnv * const env, const jclass class, const jint n)
{
  return (*env)->NewString(env, words_counts[n].str + strbase, words_counts[n].len);
}

JNIEXPORT jint JNICALL Java_us_achromaticmetaphor_agram_Word_get_1nwords
  (JNIEnv * const env, const jclass class)
{
  return NWORDS;
}

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_Anagrams_generate_1native
  (JNIEnv * const env, const jclass class, const jstring string)
{
  struct al al;
  jchar const * const str = alinit(&al, env) ? NULL : (*env)->GetStringChars(env, string, NULL);
  if (str)
    {
      anagrams(str, (*env)->GetStringLength(env, string), aladd, &al);
      (*env)->ReleaseStringChars(env, string, str);
    }
  return al.alinstance;
}

struct wr
{
  JNIEnv * env;
  jmethodID wrread;
  jobject wrinstance;
  jstring wrcur;
};

static int wr_has_next(void * const vwr)
{
  struct wr * const wr = vwr;
  if (wr->wrcur)
    (*wr->env)->DeleteLocalRef(wr->env, wr->wrcur);
  return ! ! (wr->wrcur = (*wr->env)->CallObjectMethod(wr->env, wr->wrinstance, wr->wrread));
}

static size_t wrlen(void * const vwr)
{
  struct wr * const wr = vwr;
  return (*wr->env)->GetStringLength(wr->env, wr->wrcur);
}

static void wrget(jchar * const out, void * const vwr)
{
  struct wr * const wr = vwr;
  (*wr->env)->GetStringRegion(wr->env, wr->wrcur, 0, wrlen(vwr), out);
}

static int wr_init(struct wr * const wr, JNIEnv * const env, jobject const jwords)
{
  wr->env = env;
  wr->wrinstance = jwords;
  wr->wrcur = NULL;
  jclass const wlreader = (*env)->FindClass(env, "us/achromaticmetaphor/agram/WordlistReader");
  wr->wrread = wlreader ? (*env)->GetMethodID(env, wlreader, "read", "()Ljava/lang/String;") : NULL;
  return ! wr->wrread;
}

JNIEXPORT jboolean JNICALL Java_us_achromaticmetaphor_agram_Native_init__Ljava_lang_String_2Lus_achromaticmetaphor_agram_WordlistReader_2
  (JNIEnv * const env, jclass const class, jstring const jfn, jobject const jwords)
{
  struct wr wr;
  static const struct cwlcbs wrcbs = {wr_has_next, wrlen, wrget};
  const char * const fn = wr_init(&wr, env, jwords) ? NULL : (*env)->GetStringUTFChars(env, jfn, 0);
  const int compile_failed = fn ? compile_wl(fn, &wrcbs, &wr) : 1;
  const int load_failed = compile_failed ? 1 : load_wl(fn);
  if (fn)
    (*env)->ReleaseStringUTFChars(env, jfn, fn);
  return load_failed ? JNI_FALSE : JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_us_achromaticmetaphor_agram_Native_init__Ljava_lang_String_2
  (JNIEnv * const env, jclass const class, jstring const jfn)
{
  const char * const fn = (*env)->GetStringUTFChars(env, jfn, 0);
  const int failure = fn ? load_wl(fn) : 1;
  if (fn)
    (*env)->ReleaseStringUTFChars(env, jfn, fn);
  return failure ? JNI_FALSE : JNI_TRUE;
}
