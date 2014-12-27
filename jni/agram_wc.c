#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "lcwc.h"

size_t NWORDS = 0;

struct lc * words_counts;
size_t words_counts_len = 0;

char * strbase;
size_t strbase_len = 0;

char * charsbase;
size_t charsbase_len = 0;

unsigned int * countsbase;
size_t countsbase_len = 0;

static void unload_wl_s(const int n)
{
  switch(n)
    {
      default:
      case 5:
        munmap(countsbase, countsbase_len);
        countsbase_len = 0;
      case 4:
        munmap(charsbase, charsbase_len);
        charsbase_len = 0;
      case 3:
        munmap(strbase, strbase_len);
        strbase_len = 0;
      case 2:
        munmap(words_counts, words_counts_len);
        words_counts_len = 0;
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

static void * omap(const char * const fn, size_t * const len)
{
  const int fd = open(fn, O_RDONLY);
  if (fd == -1)
    return MAP_FAILED;

  struct stat st;
  if (fstat(fd, &st))
    {
      close(fd);
      return MAP_FAILED;
    }

  void * const mapping = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);
  *len = st.st_size;
  return mapping;
}

int load_wl(const char * const fn)
{
  const int fd = open(fn, O_RDONLY);
  if (fd == -1)
    return 1;

  const ssize_t bread = read(fd, &NWORDS, sizeof(NWORDS));
  close(fd);
  if (bread != sizeof(NWORDS))
    return unload_wl_s(1), 1;

  char fne[strlen(fn)+3];
  sprintf(fne, "%s.i", fn);
  words_counts = omap(fne, &words_counts_len);
  if (words_counts == MAP_FAILED)
    return unload_wl_s(1), 1;

  sprintf(fne, "%s.s", fn);
  strbase = omap(fne, &strbase_len);
  if (strbase == MAP_FAILED)
    return unload_wl_s(2), 1;

  sprintf(fne, "%s.c", fn);
  charsbase = omap(fne, &charsbase_len);
  if (charsbase == MAP_FAILED)
    return unload_wl_s(3), 1;

  sprintf(fne, "%s.n", fn);
  countsbase = omap(fne, &countsbase_len);
  if (countsbase == MAP_FAILED)
    return unload_wl_s(4), 1;

  return 0;
}
