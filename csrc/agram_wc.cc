#define _POSIX_C_SOURCE 200809L

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <vector>

#include "agram_types.h"
#include "lcwc.h"
#include "wordlist.h"

struct file
{
  FILE * fh;

  file(char const * const fn) { fh = fopen(fn, "rb"); }
  ~file()
  {
    if (fh != nullptr)
      fclose(fh);
  }
};

template <typename T>
static bool read_file(std::vector<T> & out, char const * const fn, unsigned long const nelems)
{
  file fh(fn);
  if (fh.fh == nullptr)
    return false;
  out.resize(nelems);
  return fread(out.data(), sizeof(T), nelems, fh.fh) == nelems;
}

template <char E, typename T>
static bool read_file_ext(std::vector<T> & out, char const * const fn, long const tell)
{
  char fne[strlen(fn) + 3];
  sprintf(fne, "%s.%c", fn, E);
  return read_file(out, fne, tell / sizeof(T));
}

int load_wl(struct wordlist * const wl, const char * const fn)
{
  long tells[4];
  FILE * const fi = fopen(fn, "rb");
  if (!fi)
    return 1;

  agram_size nwords;
  const int failed = fread(&nwords, sizeof(nwords), 1, fi) != 1 || fread(tells, sizeof(*tells), 4, fi) != 4;
  fclose(fi);
  if (failed)
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
