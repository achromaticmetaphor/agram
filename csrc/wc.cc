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

struct cwlsink
{
  virtual int each(lc const *, agram_dchar const *, agram_cpt const *, unsigned int const *) = 0;
  virtual int all() = 0;

  int compile(cwlsrc & src);
};

int cwlsink::compile(cwlsrc & src)
{
  agram_size NWORDS = 0;
  size_t stroff = 0;
  size_t charsoff = 0;
  std::vector<unsigned int> counts;
  std::vector<agram_cpt> chars;
  while (src.has_next())
    {
      struct lc index;
      NWORDS++;
      index.len = src.len();
      agram_dchar str[index.len + 1];
      src.get(str);
      counts.clear();
      chars.clear();
      lettercounts(counts, chars, str, index.len);
      index.nchars = chars.size();
      index.str = stroff;
      index.chars = charsoff;
      index.hash = wc_hash_chars(chars);
      stroff += index.len;
      charsoff += index.nchars;

      if (each(&index, str, chars.data(), counts.data()))
        return 1;
    }

  return all();
}

struct file_sink : cwlsink
{
  FILE * i;
  FILE * s;
  FILE * c;
  FILE * n;
  FILE * b;
  agram_size NWORDS;

  int each(lc const *, agram_dchar const *, agram_cpt const *, unsigned int const *);
  int all();
};

int file_sink::each(lc const * const index, agram_dchar const * const str, agram_cpt const * const chars, unsigned int const * const counts)
{
  NWORDS++;
  return fwrite(index, sizeof(*index), 1, i) != 1 || fwrite(str, sizeof(*str), index->len, s) != index->len || fwrite(chars, sizeof(*chars), index->nchars, c) != index->nchars || fwrite(counts, sizeof(*counts), index->nchars, n) != index->nchars;
}

int file_sink::all()
{
  std::array<long, 4> tells = {ftell(i), ftell(s), ftell(c), ftell(n)};
  for (long t : tells)
    if (t == -1)
      return 1;

  return (fwrite(&NWORDS, sizeof(NWORDS), 1, b) != 1 || fwrite(tells.data(), sizeof(*tells.data()), tells.size(), b) != tells.size());
}

int cwlsrc::compile_wl(const char * outfn)
{
  int failed = 1;
  struct file_sink sink;
  char fne[strlen(outfn) + 3];

#define FOF(suffix, fail)             \
  sprintf(fne, "%s." #suffix, outfn); \
  sink.suffix = fopen(fne, "wb");     \
  if (!sink.suffix)                   \
    fail;

  FOF(i, return 1)
  FOF(s, goto fail_i)
  FOF(c, goto fail_s)
  FOF(n, goto fail_c)
  if (!(sink.b = fopen(outfn, "wb")))
    goto fail_n;
  sink.NWORDS = 0;

  failed = sink.compile(*this);
  failed |= fclose(sink.b);
fail_n:
  failed |= fclose(sink.n);
fail_c:
  failed |= fclose(sink.c);
fail_s:
  failed |= fclose(sink.s);
fail_i:
  failed |= fclose(sink.i);
  return failed;
}

struct mem_sink : cwlsink
{
  std::vector<lc> words_counts;
  std::vector<agram_dchar> strbase;
  std::vector<agram_cpt> charsbase;
  std::vector<unsigned int> countsbase;

  int each(lc const *, agram_dchar const *, agram_cpt const *, unsigned int const *);
  int all() { return 0; }
};

int mem_sink::each(lc const * const index, agram_dchar const * const str, agram_cpt const * const chars, unsigned int const * const counts)
{
  words_counts.push_back(*index);
  for (unsigned int i = 0; i < index->len; ++i)
    strbase.push_back(str[i]);
  for (unsigned int i = 0; i < index->nchars; ++i)
    charsbase.push_back(chars[i]);
  for (unsigned int i = 0; i < index->nchars; ++i)
    countsbase.push_back(counts[i]);
  return 0;
}

int cwlsrc::build_wl(struct wordlist * const wl)
{
  mem_sink sink;
  if (sink.compile(*this))
    return 1;

  wl->words_counts = std::move(sink.words_counts);
  wl->strbase = std::move(sink.strbase);
  wl->charsbase = std::move(sink.charsbase);
  wl->countsbase = std::move(sink.countsbase);
  return 0;
}
