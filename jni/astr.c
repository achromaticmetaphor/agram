#include <stddef.h>
#include <stdlib.h>

#include <jni.h>

size_t astrlen(jint const * str)
{
  jint const * end = str;
  while (*end)
    end++;
  return end - str;
}

void astrcpy(jint * dest, const jint * src)
{
  while (*dest++ = *src++);
}

jint * astrdup(jint const * str)
{
  jint * out = malloc(sizeof(*out) * (astrlen(str) + 1));
  if (out)
    astrcpy(out, str);
  return out;
}
