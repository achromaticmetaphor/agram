#include <errno.h>
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

static int putout(const int fd, const void * buf, size_t len)
{
  while (len)
    {
      const ssize_t written = write(fd, buf, len);
      if (written == -1)
        if (errno == EINTR)
          continue;
        else
          return 1;
      len -= written;
      buf = (char *) buf + written;
    }
  return 0;
}

static jboolean compile_wl(JNIEnv * const env, jobject const jwords, const char * const outfn)
{
  jclass const bufread = (*env)->FindClass(env, "java/io/BufferedReader");
  jmethodID const readline = bufread ? (*env)->GetMethodID(env, bufread, "readLine", "()Ljava/lang/String;") : NULL;
  if (! readline)
    return JNI_FALSE;

  char fne[strlen(outfn)+3];
  sprintf(fne, "%s.i", outfn);
  const int fdi = creat(fne, S_IRUSR);
  if (fdi == -1)
    return JNI_FALSE;

  sprintf(fne, "%s.s", outfn);
  const int fds = creat(fne, S_IRUSR);
  if (fds == -1)
    goto fail_i;

  sprintf(fne, "%s.c", outfn);
  const int fdc = creat(fne, S_IRUSR);
  if (fdc == -1)
    goto fail_s;

  sprintf(fne, "%s.n", outfn);
  const int fdn = creat(fne, S_IRUSR);
  if (fdn == -1)
    goto fail_c;

  jint NWORDS = 0;
  size_t stroff = 0;
  size_t charsoff = 0;
  while (1)
    {
      struct lc index;
      jstring const string = (*env)->CallObjectMethod(env, jwords, readline);
      if ((*env)->ExceptionCheck(env) == JNI_TRUE)
        goto fail_n;
      if (! string)
        break;
      NWORDS++;
      index.len = (*env)->GetStringLength(env, string);
      if (index.len == 0)
        continue;
      jchar str[index.len];
      jint counts[index.len];
      jint chars[index.len];
      (*env)->GetStringRegion(env, string, 0, index.len, str);
      (*env)->DeleteLocalRef(env, string);
      index.nchars = lettercounts(counts, chars, str, index.len);
      index.str = stroff;
      index.chars = charsoff;
      stroff += index.len;
      charsoff += index.nchars;

      if (putout(fdi, &index, sizeof(index))
       || putout(fds, str, sizeof(str))
       || putout(fdc, chars, sizeof(*chars) * index.nchars)
       || putout(fdn, counts, sizeof(*counts) * index.nchars))
        goto fail_n;
    }

  if (fsync(fdn) | close(fdn)
    | fsync(fdc) | close(fdc)
    | fsync(fds) | close(fds)
    | fsync(fdi) | close(fdi))
    return JNI_FALSE;

  const int fd = creat(outfn, S_IRUSR);
  if (putout(fd, &NWORDS, sizeof(NWORDS)) | fsync(fd) | close(fd))
    return JNI_FALSE;

  sprintf(fne, "%s.k", outfn);
  const int fdk = creat(fne, S_IRUSR);
  fsync(fdk);
  close(fdk);

  return JNI_TRUE;

fail_n:
  close(fdn);
fail_c:
  close(fdc);
fail_s:
  close(fds);
fail_i:
  close(fdi);
  return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL Java_us_achromaticmetaphor_agram_Native_init__Ljava_lang_String_2Ljava_io_BufferedReader_2
  (JNIEnv * const env, jclass const class, jstring const jfn, jobject const jwords)
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
