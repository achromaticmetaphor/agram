#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "agram_wc.h"
#include "astr.h"
#include "lcwc.h"
#include "lettercounts.h"

#include <jni.h>

static size_t wllen(JNIEnv * const env, jobjectArray const jwords, jsize const nwords)
{
  size_t sum = 0;
  size_t i;
  for (i = 0; i < nwords; i++)
    {
      jstring const string = (*env)->GetObjectArrayElement(env, jwords, i);
      sum += (*env)->GetStringLength(env, string);
      (*env)->DeleteLocalRef(env, string);
    }
  return sum;
}

static void * mapping(const char * const fn, const size_t len)
{
  const int fd = open(fn, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR);
  void * const mapping = ftruncate(fd, len) ? MAP_FAILED : mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  return mapping;
}

static jboolean compile_wl(JNIEnv * const env, jobjectArray const jwords, const char * const outfn)
{
  const jsize NWORDS = (*env)->GetArrayLength(env, jwords);

  const int fd = creat(outfn, S_IRUSR);
  if (write(fd, &NWORDS, sizeof(NWORDS)) != sizeof(NWORDS) || fsync(fd) || close(fd))
    return JNI_FALSE;

  char fne[strlen(outfn)+3];
  sprintf(fne, "%s.i", outfn);
  struct lc * const index = mapping(fne, sizeof(*index) * NWORDS);
  if (index == MAP_FAILED)
    return JNI_FALSE;

  const size_t len = wllen(env, jwords, NWORDS);
  sprintf(fne, "%s.s", outfn);
  jchar * const str = mapping(fne, sizeof(*str) * len);
  if (str == MAP_FAILED)
    {
      munmap(index, sizeof(*index) * NWORDS);
      return JNI_FALSE;
    }

  sprintf(fne, "%s.c", outfn);
  jint * const chars = mapping(fne, sizeof(*chars) * len);
  if (chars == MAP_FAILED)
    {
      munmap(str, len);
      munmap(index, sizeof(*index) * NWORDS);
      return JNI_FALSE;
    }

  sprintf(fne, "%s.n", outfn);
  unsigned int * const counts = mapping(fne, len * sizeof(*counts));
  if (counts == MAP_FAILED)
    {
      munmap(chars, len);
      munmap(str, len);
      munmap(index, sizeof(*index) * NWORDS);
      return JNI_FALSE;
    }

  size_t stroff = 0;
  size_t charsoff = 0;
  size_t i;
  for (i = 0; i < NWORDS; i++)
    {
      jstring const string = (*env)->GetObjectArrayElement(env, jwords, i);
      index[i].len = (*env)->GetStringLength(env, string);
      (*env)->GetStringRegion(env, string, 0, index[i].len, str + stroff);
      (*env)->DeleteLocalRef(env, string);
      index[i].nchars = lettercounts(counts + charsoff, chars + charsoff, str + stroff, index[i].len);
      index[i].str = stroff;
      index[i].chars = charsoff;
      stroff += index[i].len;
      charsoff += index[i].nchars;
    }

  munmap(index, sizeof(*index) * NWORDS);
  munmap(str, len);
  munmap(chars, len);
  munmap(counts, len * sizeof(*counts));

  sprintf(fne, "%s.k", outfn);
  const int fdk = creat(fne, S_IRUSR);
  fsync(fdk);
  close(fdk);

  return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_us_achromaticmetaphor_agram_Native_init__Ljava_lang_String_2_3Ljava_lang_String_2
  (JNIEnv * const env, jclass const class, jstring const jfn, jobjectArray const jwords)
{
  const char * const fn = (*env)->GetStringUTFChars(env, jfn, 0);
  const int compile_failed = fn ? ! compile_wl(env, jwords, fn) : 1;
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
