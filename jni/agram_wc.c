#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agram_types.h"
#include "lcwc.h"

#if AGRAM_MMAP
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

agram_size NWORDS = 0;

struct lc * words_counts = NULL;
size_t words_counts_len = 0;

agram_dchar * strbase = NULL;
size_t strbase_len = 0;

agram_cpt * charsbase = NULL;
size_t charsbase_len = 0;

unsigned int * countsbase = NULL;
size_t countsbase_len = 0;

static void unload_wl_s(const int n)
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
        MUNMAP(countsbase)
      case 4:
        MUNMAP(charsbase)
      case 3:
        MUNMAP(strbase)
      case 2:
        MUNMAP(words_counts)
      case 1:
        NWORDS = 0;
      case 0:
        ;
    }
}

void unload_wl(void)
{
  unload_wl_s(5);
}

static void * omap(const char * const fn, long const tell)
{
#if AGRAM_MMAP
  int fd;
  struct stat st;
  void * const mapping = (fd = open(fn, O_RDONLY)) != -1 && ! fstat(fd, &st) && st.st_size == tell ? mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0) : MAP_FAILED;
  close(fd);
  return mapping == MAP_FAILED ? NULL : mapping;
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
        return free(mapping), NULL;
    }
  return mapping;
#endif
}

int load_wl(const char * const fn)
{
  unload_wl();
  long tells[4];
  FILE * const fi = fopen(fn, "rb");
  if (! fi)
    return 1;

  const int failed = fread(&NWORDS, sizeof(NWORDS), 1, fi) != 1 || fread(tells, sizeof(*tells), 4, fi) != 4;
  fclose(fi);
  if (failed)
    return unload_wl_s(1), 1;

  char fne[strlen(fn)+3];

#define OMAP(suffix, mapping, depth, tell) \
  sprintf(fne, "%s." # suffix, fn); \
  mapping = omap(fne, tell); \
  if (! mapping) \
    return unload_wl_s(depth), 1; \
  mapping ## _len = tell;

  OMAP(i, words_counts, 1, tells[0])
  OMAP(s, strbase, 2, tells[1])
  OMAP(c, charsbase, 3, tells[2])
  OMAP(n, countsbase, 4, tells[3])

  return 0;
}
