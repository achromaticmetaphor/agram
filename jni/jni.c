#include <stddef.h>
#include <stdlib.h>

#include <jni.h>

#include "agram_wc.h"
#include "anagram.h"
#include "anagrams.h"
#include "wc.h"
#include "wl_upgrade.h"
#include "wordlist.h"
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

static void * get_marshalled_pointer_or_null(JNIEnv * const env, jobject const obj, const char * const field)
{
  jfieldID const handlefield = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, obj), field, "[B");
  jbyteArray const handle = handlefield ? (*env)->GetObjectField(env, obj, handlefield) : NULL;
  if (! handle || (*env)->GetArrayLength(env, handle) != sizeof(void *) / sizeof(jbyte))
    return NULL;
  void * pointer;
  (*env)->GetByteArrayRegion(env, handle, 0, sizeof(void *) / sizeof(jbyte), (void *) &pointer);
  return pointer;
}

static void * get_marshalled_pointer(JNIEnv * const env, jobject const obj, const char * const field)
{
  void * pointer = get_marshalled_pointer_or_null(env, obj, field);
  if (! pointer)
    (*env)->ThrowNew(env, (*env)->FindClass(env, "java/lang/IllegalStateException"), "null or invalid handle");
  return pointer;
}

static int put_marshalled_pointer(JNIEnv * const env, jobject const obj, const char * const field, const void * const pointer)
{
  jfieldID const handlefield = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, obj), field, "[B");
  jbyteArray const handle = handlefield ? (*env)->NewByteArray(env, sizeof(void *) / sizeof(jbyte)) : NULL;
  if (! handle)
    return 1;

  (*env)->SetByteArrayRegion(env, handle, 0, sizeof(void *) / sizeof(jbyte), (void *) &pointer);
  (*env)->SetObjectField(env, obj, handlefield, handle);
  return 0;
}

static void clear_marshalled_pointer(JNIEnv * const env, jobject const obj, const char * const field)
{
  jfieldID const handlefield = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, obj), field, "[B");
  if (handlefield)
    (*env)->SetObjectField(env, obj, handlefield, NULL);
}

static const struct wordlist * get_wordlist_handle(JNIEnv * const env, jobject const obj)
{
  jfieldID const wordlistField = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, obj), "wordlist", "Lus/achromaticmetaphor/agram/Wordlist;");
  jobject const wordlist = wordlistField ? (*env)->GetObjectField(env, obj, wordlistField) : NULL;
  const struct wordlist * const wlhandle = wordlist ? get_marshalled_pointer_or_null(env, wordlist, "wordlist_handle") : NULL;
  if (! wlhandle)
    (*env)->ThrowNew(env, (*env)->FindClass(env, "java/lang/IllegalStateException"), "null or invalid handle");
  return wlhandle;
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

static int stradd(JNIEnv * const env, jobject const obj, jmethodID const mid, jchar const * str, size_t const len)
{
  jstring const s = (*env)->NewString(env, str, len);
  if (! s)
    return 1;
  (*env)->CallBooleanMethod(env, obj, mid, s);
  (*env)->DeleteLocalRef(env, s);
  return 0;
}

static int aladd(jchar const * str, size_t const len, void * const val)
{
  struct al * const al = val;
  return stradd(al->env, al->alinstance, al->aladd, str, len);
}

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_Anagram_generate
  (JNIEnv * const env, const jobject obj, const jstring string)
{
  const struct wordlist * const wl = get_wordlist_handle(env, obj);
  if (! wl)
    return NULL;
  struct al al;
  jchar const * const str = alinit(&al, env) ? NULL : (*env)->GetStringChars(env, string, NULL);
  if (str)
    {
      anagram(wl, str, (*env)->GetStringLength(env, string), aladd, &al);
      (*env)->ReleaseStringChars(env, string, str);
    }
  return al.alinstance;
}

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_WordsFrom_generate
  (JNIEnv * const env, const jobject obj, const jstring string, const jboolean max)
{
  const struct wordlist * const wl = get_wordlist_handle(env, obj);
  if (! wl)
    return NULL;
  struct al al;
  jchar const * const str = alinit(&al, env) ? NULL : (*env)->GetStringChars(env, string, NULL);
  if (str)
    {
      words_from(wl, str, (*env)->GetStringLength(env, string), max, aladd, &al);
      (*env)->ReleaseStringChars(env, string, str);
    }
  return al.alinstance;
}

JNIEXPORT jstring JNICALL Java_us_achromaticmetaphor_agram_Word_pick_1native
  (JNIEnv * const env, const jclass class, jobject const wordlist, const jint n)
{
  const struct wordlist * const wl = get_marshalled_pointer(env, wordlist, "wordlist_handle");
  if (! wl)
    return NULL;
  return (*env)->NewString(env, wl->words_counts[n].str + wl->strbase, wl->words_counts[n].len);
}

JNIEXPORT jint JNICALL Java_us_achromaticmetaphor_agram_Wordlist_get_1nwords
  (JNIEnv * const env, const jobject obj)
{
  const struct wordlist * const wl = get_marshalled_pointer(env, obj, "wordlist_handle");
  if (! wl)
    return 0;
  return wl->nwords;
}

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_Anagrams_generate__Ljava_lang_String_2
  (JNIEnv * const env, const jobject obj, const jstring string)
{
  const struct wordlist * const wl = get_wordlist_handle(env, obj);
  if (! wl)
    return NULL;
  struct al al;
  jchar const * const str = alinit(&al, env) ? NULL : (*env)->GetStringChars(env, string, NULL);
  if (str)
    {
      anagrams(wl, str, (*env)->GetStringLength(env, string), aladd, &al);
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

JNIEXPORT void JNICALL Java_us_achromaticmetaphor_agram_Wordlist_loadNullWordlist
  (JNIEnv * const env, jobject const obj)
{
  static const struct wordlist nullwl = {0};
  put_marshalled_pointer(env, obj, "wordlist_handle", &nullwl);
}

JNIEXPORT jboolean JNICALL Java_us_achromaticmetaphor_agram_Wordlist_init__Ljava_lang_String_2Lus_achromaticmetaphor_agram_WordlistReader_2
  (JNIEnv * const env, jobject const obj, jstring const jfn, jobject const jwords)
{
  struct wr wr;
  static const struct cwlcbs wrcbs = {wr_has_next, wrlen, wrget};
  const char * const fn = wr_init(&wr, env, jwords) ? NULL : (*env)->GetStringUTFChars(env, jfn, 0);
  const int compile_failed = fn ? compile_wl(fn, &wrcbs, &wr) : 1;
  struct wordlist * const wl = compile_failed ? NULL : malloc(sizeof(*wl));
  const int load_failed = wl ? load_wl(wl, fn) : 1;
  if (fn)
    (*env)->ReleaseStringUTFChars(env, jfn, fn);
  return load_failed || put_marshalled_pointer(env, obj, "wordlist_handle", wl) ? JNI_FALSE : JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_us_achromaticmetaphor_agram_Wordlist_init__Ljava_lang_String_2
  (JNIEnv * const env, jobject const obj, jstring const jfn)
{
  const char * const fn = (*env)->GetStringUTFChars(env, jfn, 0);
  struct wordlist * const wl = fn ? malloc(sizeof(*wl)) : NULL;
  const int failure = wl ? load_wl(wl, fn) : 1;
  if (fn)
    (*env)->ReleaseStringUTFChars(env, jfn, fn);
  return failure || put_marshalled_pointer(env, obj, "wordlist_handle", wl) ? JNI_FALSE : JNI_TRUE;
}

JNIEXPORT void JNICALL Java_us_achromaticmetaphor_agram_Anagrams_uninit
  (JNIEnv * const env, jobject const obj)
{
  struct agsto * state = get_marshalled_pointer_or_null(env, obj, "handle");
  clear_marshalled_pointer(env, obj, "handle");
  anagrams_destroy(state);
  free(state);
}

JNIEXPORT jboolean JNICALL Java_us_achromaticmetaphor_agram_Anagrams_init__Ljava_lang_String_2
  (JNIEnv * const env, jobject const obj, jstring const jstr)
{
  Java_us_achromaticmetaphor_agram_Anagrams_uninit(env, obj);
  const struct wordlist * const wl = get_wordlist_handle(env, obj);
  if (! wl)
    return JNI_FALSE;

  struct agsto * const state = malloc(sizeof(*state));
  jchar const * const str = state ? (*env)->GetStringChars(env, jstr, NULL) : NULL;
  int const init_failed = str ? anagrams_init(state, wl, str, (*env)->GetStringLength(env, jstr)) : 1;
  if (str)
    (*env)->ReleaseStringChars(env, jstr, str);

  if (init_failed || put_marshalled_pointer(env, obj, "handle", state))
    {
      anagrams_destroy(state);
      free(state);
      return JNI_FALSE;
    }
  else
    return JNI_TRUE;
}

JNIEXPORT jobject JNICALL Java_us_achromaticmetaphor_agram_Anagrams_generate__I
  (JNIEnv * const env, jobject const obj, jint const n)
{
  struct al al;
  if (alinit(&al, env))
    return NULL;

  struct agsto * state = get_marshalled_pointer(env, obj, "handle");
  if (! state)
    return NULL;

  jint i;
  for (i = 0; i < n; i++)
    {
      size_t const slen = anagrams_single(state);
      if (! slen)
        break;
      if (aladd(state->prefix + 1, slen, &al))
        break;
    }
  return al.alinstance;
}

JNIEXPORT jboolean JNICALL Java_us_achromaticmetaphor_agram_Wordlist_upgrade
  (JNIEnv * const env, jclass const class, jstring const jfnold, jstring const jfnnew, jbyte const version)
{
  if (! agram_wl_can_upgrade(version))
    return JNI_FALSE;

  const char * const fnold = (*env)->GetStringUTFChars(env, jfnold, 0);
  const char * const fnnew = fnold ? (*env)->GetStringUTFChars(env, jfnnew, 0) : NULL;

  int const failure = fnnew ? agram_wl_upgrade(fnold, fnnew, version) : 1;

  if (fnnew)
    (*env)->ReleaseStringUTFChars(env, jfnnew, fnnew);
  if (fnold)
    (*env)->ReleaseStringUTFChars(env, jfnold, fnold);

  return failure ? JNI_FALSE : JNI_TRUE;
}
