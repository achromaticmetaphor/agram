#include <cstddef>
#include <fstream>
#include <vector>

#include "agram_types.h"
#include "wordlist.h"

template <typename T>
static bool read_file(std::vector<T> & out, std::string const & fn,
                      unsigned long const nelems)
{
  std::ifstream fh(fn, std::ios::binary);
  out.resize(nelems);
  fh.read(reinterpret_cast<char *>(out.data()), sizeof(T) * nelems);
  return !fh.good();
}

template <char E, typename T>
static bool read_file_ext(std::vector<T> & out, char const * const fn,
                          long const tell)
{
  std::string path = std::string(fn) + '.' + E;
  return read_file(out, path, tell / sizeof(T));
}

int load_wl(struct wordlist * const wl, const char * const fn)
{
  long tells[4];
  agram_size nwords;
  std::ifstream fi(fn, std::ios::binary);

  fi.read(reinterpret_cast<char *>(&nwords), sizeof(nwords));
  fi.read(reinterpret_cast<char *>(tells), sizeof(*tells) * 4);
  if (!fi.good())
    return 1;

  if (read_file_ext<'i'>(wl->words_counts, fn, tells[0]))
    return 1;
  if (read_file_ext<'s'>(wl->strbase, fn, tells[1]))
    return 1;
  if (read_file_ext<'c'>(wl->charsbase, fn, tells[2]))
    return 1;
  if (read_file_ext<'n'>(wl->countsbase, fn, tells[3]))
    return 1;

  return 0;
}
