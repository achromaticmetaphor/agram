#include <array>
#include <cstddef>
#include <fstream>
#include <utility>
#include <vector>

#include "agram_types.h"
#include "agram_wc.h"
#include "anagram_target.h"
#include "lettercounts.h"
#include "wordlist_source.h"

struct wordlist_sink
{
  virtual int each(wordlist_entry const *, agram_display_char const *,
                   agram_codepoint const *, unsigned int const *) = 0;
  virtual int all() = 0;

  int compile(wordlist_source & src);
};

int wordlist_sink::compile(wordlist_source & src)
{
  agram_size NWORDS = 0;
  size_t stroff = 0;
  size_t charsoff = 0;
  std::vector<unsigned int> counts;
  std::vector<agram_codepoint> chars;
  while (src.has_next())
    {
      wordlist_entry index;
      NWORDS++;
      index.len = src.len();
      agram_display_char const * const str = src.get();
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

struct file_sink : wordlist_sink
{
  std::ofstream i;
  std::ofstream s;
  std::ofstream c;
  std::ofstream n;
  std::ofstream b;
  agram_size NWORDS;

  int each(wordlist_entry const *, agram_display_char const *,
           agram_codepoint const *, unsigned int const *);
  int all();

  file_sink(std::string const & base)
      : i(base + ".i", std::ios::binary), s(base + ".s", std::ios::binary),
        c(base + ".c", std::ios::binary), n(base + ".n", std::ios::binary),
        b(base, std::ios::binary), NWORDS(0)
  {
  }
};

template <typename T>
static inline void swrite(std::ofstream & out, T const * dat, size_t nelems)
{
  out.write(reinterpret_cast<char const *>(dat), sizeof(*dat) * nelems);
}

template <typename T>
static inline void swrite(std::ofstream & out, T const & con)
{
  swrite(out, con.data(), con.size());
}

template <typename T, typename U, size_t N>
static constexpr std::array<T, N> sadcast(std::array<U, N> const & src)
{
  std::array<T, N> dst = {};
  for (size_t i = 0; i < N; ++i)
    dst[i] = static_cast<T>(src[i]);
  return std::move(dst);
}

int file_sink::each(wordlist_entry const * const index,
                    agram_display_char const * const str,
                    agram_codepoint const * const chars,
                    unsigned int const * const counts)
{
  NWORDS++;
  swrite(i, index, 1);
  swrite(s, str, index->len);
  swrite(c, chars, index->nchars);
  swrite(n, counts, index->nchars);
  return !(i.good() && s.good() && c.good() && n.good());
}

int file_sink::all()
{
  std::array<std::ofstream::pos_type, 4> tells = {i.tellp(), s.tellp(),
                                                  c.tellp(), n.tellp()};
  for (auto t : tells)
    if (t == -1)
      return 1;

  auto ltells = sadcast<long>(tells);
  swrite(b, &NWORDS, 1);
  swrite(b, ltells);

  std::array<std::ofstream *, 5> streams = {&i, &s, &c, &n, &b};
  for (auto st : streams)
    {
      st->flush();
      if (!st->good())
        return 1;
    }
  return 0;
}

int wordlist_source::compile_wl(const char * outfn)
{
  return file_sink(outfn).compile(*this);
}

struct mem_sink : wordlist_sink
{
  std::vector<wordlist_entry> words_counts;
  std::vector<agram_display_char> strbase;
  std::vector<agram_codepoint> charsbase;
  std::vector<unsigned int> countsbase;

  int each(wordlist_entry const *, agram_display_char const *,
           agram_codepoint const *, unsigned int const *);
  int all() { return 0; }
};

int mem_sink::each(wordlist_entry const * const index,
                   agram_display_char const * const str,
                   agram_codepoint const * const chars,
                   unsigned int const * const counts)
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

int wordlist_source::build_wl(wordlist * const wl)
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
