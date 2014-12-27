#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

#include <jni.h>

#include "lettercounts.h"
#include "lcwc.h"
#include "jnihelp.h"

static size_t wllen(const char * const words[], const size_t nwords)
{
  size_t sum = 0;
  size_t i;
  for (i = 0; i < nwords; i++)
    sum += strlen(words[i]) + 1;
  return sum;
}

static void * mapping(const char * const fn, const size_t len)
{
  const int fd = open(fn, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR);
  void * const mapping = ftruncate(fd, len) ? MAP_FAILED : mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  return mapping;
}

static jboolean compile_wl(const char * const words[], const size_t NWORDS, const char * const outfn)
{
  const int fd = creat(outfn, S_IRUSR);
  if (write(fd, &NWORDS, sizeof(NWORDS)) != sizeof(NWORDS) || fsync(fd) || close(fd))
    return JNI_FALSE;

  char fne[strlen(outfn)+3];
  sprintf(fne, "%s.i", outfn);
  struct lc * const index = mapping(fne, sizeof(*index) * NWORDS);
  if (index == MAP_FAILED)
    return JNI_FALSE;

  const size_t len = wllen(words, NWORDS);
  sprintf(fne, "%s.s", outfn);
  char * const str = mapping(fne, len);
  if (str == MAP_FAILED)
    {
      munmap(index, sizeof(*index) * NWORDS);
      return JNI_FALSE;
    }

  sprintf(fne, "%s.c", outfn);
  char * const chars = mapping(fne, len);
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
      index[i].len = strlen(words[i]);
      lettercounts(counts + charsoff, chars + charsoff, words[i]);
      index[i].nchars = strlen(chars + charsoff);
      index[i].str = stroff;
      index[i].chars = charsoff;
      strcpy(str + stroff, words[i]);
      stroff += index[i].len + 1;
      charsoff += index[i].nchars + 1;
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
  size_t nwords;
  const char * const fn = (*env)->GetStringUTFChars(env, jfn, 0);
  const char * * const words = fn ? jarr2arr(env, jwords, &nwords) : 0;
  const int compile_failed = words ? ! compile_wl(words, nwords, fn) : 1;
  const int load_failed = compile_failed ? 1 : load_wl(fn);
  if (words)
    free_jarr(env, jwords, words);
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
