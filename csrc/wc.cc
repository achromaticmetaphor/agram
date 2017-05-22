#define _POSIX_C_SOURCE 200809L

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <vector>

#include "agram_types.h"
#include "agram_wc.h"
#include "lcwc.h"
#include "lettercounts.h"
#include "wc.h"

struct cwlocbs
{
  int (*each)(void *, struct lc const *, agram_dchar const *, agram_cpt const *, unsigned int const *);
  int (*all)(void *);
};

static int compile_wl_s(cwlsrc * const src, struct cwlocbs const * const ocbs, void * const ocba)
{
  agram_size NWORDS = 0;
  size_t stroff = 0;
  size_t charsoff = 0;
  std::vector<unsigned int> counts;
  std::vector<agram_cpt> chars;
  while (src->has_next())
    {
      struct lc index;
      NWORDS++;
      index.len = src->len();
      agram_dchar str[index.len + 1];
      src->get(str);
      counts.clear();
      chars.clear();
      lettercounts(counts, chars, str, index.len);
      index.nchars = chars.size();
      index.str = stroff;
      index.chars = charsoff;
      index.hash = wc_hash_chars(chars);
      stroff += index.len;
      charsoff += index.nchars;

      if (ocbs->each(ocba, &index, str, chars.data(), counts.data()))
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
  file_ostate * const ostate = static_cast<file_ostate *>(vostate);
  ostate->NWORDS++;
  return fwrite(index, sizeof(*index), 1, ostate->i) != 1 || fwrite(str, sizeof(*str), index->len, ostate->s) != index->len || fwrite(chars, sizeof(*chars), index->nchars, ostate->c) != index->nchars || fwrite(counts, sizeof(*counts), index->nchars, ostate->n) != index->nchars;
}

static int file_all(void * const vostate)
{
  file_ostate * const ostate = static_cast<file_ostate *>(vostate);

  std::array<long, 4> tells = {ftell(ostate->i), ftell(ostate->s), ftell(ostate->c), ftell(ostate->n)};
  for (long t : tells)
    if (t == -1)
      return 1;

  return (fwrite(&ostate->NWORDS, sizeof(ostate->NWORDS), 1, ostate->b) != 1 || fwrite(tells.data(), sizeof(*tells.data()), tells.size(), ostate->b) != tells.size());
}

int cwlsrc::compile_wl(const char * outfn)
{
  int failed = 1;
  struct file_ostate ostate;
  char fne[strlen(outfn) + 3];

#define FOF(suffix, fail)             \
  sprintf(fne, "%s." #suffix, outfn); \
  ostate.suffix = fopen(fne, "wb");   \
  if (!ostate.suffix)                 \
    fail;

  FOF(i, return 1)
  FOF(s, goto fail_i)
  FOF(c, goto fail_s)
  FOF(n, goto fail_c)
  if (!(ostate.b = fopen(outfn, "wb")))
    goto fail_n;
  ostate.NWORDS = 0;

  static const struct cwlocbs ocbs = {file_each, file_all};
  failed = compile_wl_s(this, &ocbs, &ostate);

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
  std::vector<lc> words_counts;
  std::vector<agram_dchar> strbase;
  std::vector<agram_cpt> charsbase;
  std::vector<unsigned int> countsbase;
};

static int mem_each(void * const vostate, struct lc const * const index, agram_dchar const * const str, agram_cpt const * const chars, unsigned int const * const counts)
{
  mem_ostate & ostate = *static_cast<mem_ostate *>(vostate);

  ostate.words_counts.push_back(*index);
  for (unsigned int i = 0; i < index->len; ++i)
    ostate.strbase.push_back(str[i]);
  for (unsigned int i = 0; i < index->nchars; ++i)
    ostate.charsbase.push_back(chars[i]);
  for (unsigned int i = 0; i < index->nchars; ++i)
    ostate.countsbase.push_back(counts[i]);

  return 0;
}

static int mem_all(void * const vostate)
{
  return 0;
}

int cwlsrc::build_wl(struct wordlist * const wl)
{
  mem_ostate & ostate = *new mem_ostate; // TODO FIXME intentional memory leak of vector backing arrays pending cleanup of C code

  static const struct cwlocbs ocbs = {mem_each, mem_all};
  if (compile_wl_s(this, &ocbs, &ostate))
    return 1;
  else
    {
      wl->nwords = ostate.words_counts.size();
      wl->words_counts = ostate.words_counts.data();
      wl->strbase = ostate.strbase.data();
      wl->charsbase = ostate.charsbase.data();
      wl->countsbase = ostate.countsbase.data();
      return 0;
    }
}
