#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <jni.h>

size_t astrlen(jint const * str)
{
  jint const * end = str;
  while (*end)
    end++;
  return end - str;
}

void uastrcpy(jint * dest, const jchar * src, const jsize slen)
{
  jsize i;
  for (i = 0; i < slen; i++)
    if (src[i] >= 0xd800 && src[i] < 0xe000 && i + 1 < slen)
      {
        *dest++ = ((src[i] - 0xd800) << 10) | (src[i+1] - 0xdc00);
        i++;
      }
    else
      *dest++ = src[i];
    *dest = 0;
}

jchar * ustrdup(jchar const * str, const jsize slen)
{
  jchar * out = malloc(sizeof(*out) * slen);
  if (out)
    memcpy(out, str, slen * sizeof(*str));
  return out;
}
