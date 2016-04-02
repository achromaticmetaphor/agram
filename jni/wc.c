#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "agram_types.h"
#include "agram_wc.h"
#include "lcwc.h"
#include "lettercounts.h"
#include "vector.h"
#include "wc.h"

struct cwlocbs
{
  int (* each)(void *, struct lc const *, agram_dchar const *, agram_cpt const *, unsigned int const *);
  int (* all)(void *);
};

static int compile_wl_s(struct cwlcbs const * const cbs, void * const cba, struct cwlocbs const * const ocbs, void * const ocba)
{
  agram_size NWORDS = 0;
  size_t stroff = 0;
  size_t charsoff = 0;
  while (cbs->has_next(cba))
    {
      struct lc index;
      NWORDS++;
      index.len = cbs->len(cba);
      agram_dchar str[index.len + 1];
      unsigned int counts[index.len];
      agram_cpt chars[index.len];
      cbs->get(str, cba);
      index.nchars = lettercounts(counts, chars, str, index.len);
      index.str = stroff;
      index.chars = charsoff;
      index.hash = wc_hash_chars(chars, index.nchars);
      stroff += index.len;
      charsoff += index.nchars;

      if (ocbs->each(ocba, &index, str, chars, counts))
        return 1;
    }

  return ocbs->all(ocba);
}

struct file_ostate
{
  FILE * i;
  FILE * s;
  FILE * c;
  FILE * n;
  FILE * b;
  agram_size NWORDS;
};

static int file_each(void * const vostate, struct lc const * const index, agram_dchar const * const str, agram_cpt const * const chars, unsigned int const * const counts)
{
  struct file_ostate * const ostate = vostate;
  ostate->NWORDS++;
  return fwrite(index, sizeof(*index), 1, ostate->i) != 1
      || fwrite(str, sizeof(*str), index->len, ostate->s) != index->len
      || fwrite(chars, sizeof(*chars), index->nchars, ostate->c) != index->nchars
      || fwrite(counts, sizeof(*counts), index->nchars, ostate->n) != index->nchars;
}

static int file_all(void * const vostate)
{
  struct file_ostate * const ostate = vostate;

  long tells[4];
  tells[0] = ftell(ostate->i);
  tells[1] = ftell(ostate->s);
  tells[2] = ftell(ostate->c);
  tells[3] = ftell(ostate->n);

  int i;
  for (i = 0; i < 4; i++)
    if (tells[i] == -1)
      return 1;

  return (fwrite(&ostate->NWORDS, sizeof(ostate->NWORDS), 1, ostate->b) != 1 || fwrite(tells, sizeof(*tells), 4, ostate->b) != 4);
}

int compile_wl(const char * outfn, struct cwlcbs const * const cbs, void * const cba)
{
  int failed = 1;
  struct file_ostate ostate;
  char fne[strlen(outfn)+3];

#define FOF(suffix, fail) \
  sprintf(fne, "%s." # suffix, outfn); \
  ostate.suffix = fopen(fne, "wb"); \
  if (! ostate.suffix) \
    fail;

  FOF(i, return 1)
  FOF(s, goto fail_i)
  FOF(c, goto fail_s)
  FOF(n, goto fail_c)
  if (! (ostate.b = fopen(outfn, "wb")))
    goto fail_n;
  ostate.NWORDS = 0;

  static const struct cwlocbs ocbs = {file_each, file_all};
  failed = compile_wl_s(cbs, cba, &ocbs, &ostate);

  failed |= fclose(ostate.b);
fail_n:
  failed |= fclose(ostate.n);
fail_c:
  failed |= fclose(ostate.c);
fail_s:
  failed |= fclose(ostate.s);
fail_i:
  failed |= fclose(ostate.i);
  return failed;
}

struct mem_ostate
{
  agram_size NWORDS;
  struct vector words_counts;
  struct vector strbase;
  struct vector charsbase;
  struct vector countsbase;
};

static int mem_each(void * const vostate, struct lc const * const index, agram_dchar const * const str, agram_cpt const * const chars, unsigned int const * const counts)
{
  struct mem_ostate * const ostate = vostate;
  ostate->NWORDS++;
  return vector_append(&ostate->words_counts, index, sizeof(*index))
      || vector_append(&ostate->strbase, str, sizeof(*str) * index->len)
      || vector_append(&ostate->charsbase, chars, sizeof(*chars) * index->nchars)
      || vector_append(&ostate->countsbase, counts, sizeof(*counts) * index->nchars);
}

static int mem_all(void * const vostate)
{
  return 0;
}

int build_wl(struct wordlist * const wl, struct cwlcbs const * const cbs, void * const cba)
{
  struct mem_ostate ostate;
  ostate.NWORDS = 0;
  vector_init(&ostate.words_counts);
  vector_init(&ostate.strbase);
  vector_init(&ostate.charsbase);
  vector_init(&ostate.countsbase);

  static const struct cwlocbs ocbs = {mem_each, mem_all};
  if (compile_wl_s(cbs, cba, &ocbs, &ostate))
    {
      vector_destroy(&ostate.words_counts);
      vector_destroy(&ostate.strbase);
      vector_destroy(&ostate.charsbase);
      vector_destroy(&ostate.countsbase);
      return 1;
    }
  else
    {
      wl->nwords = ostate.NWORDS;
      wl->words_counts = (void *) ostate.words_counts.vector;
      wl->strbase = (void *) ostate.strbase.vector;
      wl->charsbase = (void *) ostate.charsbase.vector;
      wl->countsbase = (void *) ostate.countsbase.vector;
      return 0;
    }
}
