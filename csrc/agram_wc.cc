#define _POSIX_C_SOURCE 200809L

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "agram_types.h"
#include "lcwc.h"
#include "wordlist.h"

#if AGRAM_MMAP
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

static void unload_wl_s(struct wordlist * const wl, const int n)
{
  switch(n)
    {

#if AGRAM_MMAP
#define MUNMAP(mapping) \
  munmap(mapping, mapping ## _len); \
  mapping ## _len = 0;
#else
#define MUNMAP(mapping) \
  free(mapping); \
  mapping ## _len = 0;
#endif

      default:
      case 5:
        MUNMAP(wl->countsbase)
      case 4:
        MUNMAP(wl->charsbase)
      case 3:
        MUNMAP(wl->strbase)
      case 2:
        MUNMAP(wl->words_counts)
      case 1:
        wl->nwords = 0;
      case 0:
        ;
    }
}

void unload_wl(struct wordlist * const wl)
{
  unload_wl_s(wl, 5);
}

static void * omap(const char * const fn, long const tell)
{
#if AGRAM_MMAP
  int fd;
  struct stat st;
  void * const mapping = (fd = open(fn, O_RDONLY)) != -1 && ! fstat(fd, &st) && st.st_size == tell ? mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0) : MAP_FAILED;
  close(fd);
  return mapping == MAP_FAILED ? nullptr : mapping;
#else
  void * const mapping = malloc(tell);
  if (mapping)
    {
      int read = 0;
      FILE * const fi = fopen(fn, "rb");
      if (fi)
        {
          read = fread(mapping, 1, tell, fi) == tell;
          fclose(fi);
        }
      if (! read)
        return free(mapping), nullptr;
    }
  return mapping;
#endif
}

int load_wl(struct wordlist * const wl, const char * const fn)
{
  long tells[4];
  FILE * const fi = fopen(fn, "rb");
  if (! fi)
    return 1;

  const int failed = fread(&wl->nwords, sizeof(wl->nwords), 1, fi) != 1 || fread(tells, sizeof(*tells), 4, fi) != 4;
  fclose(fi);
  if (failed)
    return unload_wl_s(wl, 1), 1;

  char fne[strlen(fn)+3];

#define OMAP(suffix, wl, mapping, depth, tell) \
  sprintf(fne, "%s." # suffix, fn); \
  wl->mapping = (typeof(wl->mapping)) omap(fne, tell); \
  if (! wl->mapping) \
    return unload_wl_s(wl, depth), 1; \
  wl->mapping ## _len = tell;

  OMAP(i, wl, words_counts, 1, tells[0])
  OMAP(s, wl, strbase, 2, tells[1])
  OMAP(c, wl, charsbase, 3, tells[2])
  OMAP(n, wl, countsbase, 4, tells[3])

  return 0;
}
