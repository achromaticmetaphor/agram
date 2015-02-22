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
#include "jnihelp.h"

static size_t wllen(const jint * const words[], const size_t nwords)
{
  size_t sum = 0;
  size_t i;
  for (i = 0; i < nwords; i++)
    sum += astrlen(words[i]) + 1;
  return sum;
}

static void * mapping(const char * const fn, const size_t len)
{
  const int fd = open(fn, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR);
  void * const mapping = ftruncate(fd, len) ? MAP_FAILED : mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  return mapping;
}

static jboolean compile_wl(const jint * const words[], const size_t NWORDS, const char * const outfn)
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
  jint * const str = mapping(fne, sizeof(*str) * len);
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
      index[i].len = astrlen(words[i]);
      lettercounts(counts + charsoff, chars + charsoff, words[i]);
      index[i].nchars = astrlen(chars + charsoff);
      index[i].str = stroff;
      index[i].chars = charsoff;
      astrcpy(str + stroff, words[i]);
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

JNIEXPORT jboolean JNICALL Java_us_achromaticmetaphor_agram_Native_init__Ljava_lang_String_2_3_3I
  (JNIEnv * const env, jclass const class, jstring const jfn, jobjectArray const jwords)
{
  size_t nwords;
  const char * const fn = (*env)->GetStringUTFChars(env, jfn, 0);
  const jint * * const words = fn ? jarr2arr(env, jwords, &nwords) : 0;
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
