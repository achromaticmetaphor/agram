#include <stddef.h>

#include "agram_types.h"

size_t uastrcpy(agram_cpt * dest, const agram_dchar * const src, const size_t slen)
{
  jint * const dstart = dest;
  size_t i;
  for (i = 0; i < slen; i++)
    if (src[i] >= 0xd800 && src[i] < 0xe000 && i + 1 < slen)
      {
        *dest++ = ((src[i] - 0xd800) << 10) | (src[i+1] - 0xdc00);
        i++;
      }
    else
      *dest++ = src[i];
    *dest = 0;
  return dest - dstart;
}
