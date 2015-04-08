#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "lcwc.h"
#include "lettercounts.h"
#include "wc.h"

int compile_wl(const char * const outfn, struct cwlcbs const * const cbs, void * const cba)
{
  char fne[strlen(outfn)+3];

#define FOF(suffix, fail) \
  sprintf(fne, "%s." # suffix, outfn); \
  FILE * const fi ## suffix = fopen(fne, "wb"); \
  if (! fi ## suffix) \
    fail;

  FOF(i, return 1)
  FOF(s, goto fail_i)
  FOF(c, goto fail_s)
  FOF(n, goto fail_c)

  agram_size NWORDS = 0;
  size_t stroff = 0;
  size_t charsoff = 0;
  while (cbs->has_next(cba))
    {
      struct lc index;
      NWORDS++;
      index.len = cbs->len(cba);
      agram_dchar str[index.len];
      unsigned int counts[index.len];
      agram_cpt chars[index.len];
      cbs->get(str, cba);
      index.nchars = lettercounts(counts, chars, str, index.len);
      index.str = stroff;
      index.chars = charsoff;
      stroff += index.len;
      charsoff += index.nchars;

      if (fwrite(&index, sizeof(index), 1, fii) != 1
       || fwrite(str, sizeof(*str), index.len, fis) != index.len
       || fwrite(chars, sizeof(*chars), index.nchars, fic) != index.nchars
       || fwrite(counts, sizeof(*counts), index.nchars, fin) != index.nchars)
        goto fail_n;
    }

  long tells[4];
  tells[0] = ftell(fii);
  tells[1] = ftell(fis);
  tells[2] = ftell(fic);
  tells[3] = ftell(fin);

  if (fclose(fin) | fclose(fic) | fclose(fis) | fclose(fii))
    return 1;

  int i;
  for (i = 0; i < 4; i++)
    if (tells[i] == -1)
      return 1;

  FILE * const fi = fopen(outfn, "wb");
  if (! fi)
    return 1;
  if ((fwrite(&NWORDS, sizeof(NWORDS), 1, fi) != 1 || fwrite(tells, sizeof(*tells), 4, fi) != 4) | fclose(fi))
    return 1;

  return 0;

fail_n:
  fclose(fin);
fail_c:
  fclose(fic);
fail_s:
  fclose(fis);
fail_i:
  fclose(fii);
  return 1;
}
