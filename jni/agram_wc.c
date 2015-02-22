#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lcwc.h"

#include <jni.h>

size_t NWORDS = 0;

struct lc * words_counts;
static size_t words_counts_len = 0;

jint * strbase;
static size_t strbase_len = 0;

jint * charsbase;
static size_t charsbase_len = 0;

unsigned int * countsbase;
static size_t countsbase_len = 0;

static void unload_wl_s(const int n)
{
  switch(n)
    {

#define MUNMAP(mapping) \
  munmap(mapping, mapping ## _len); \
  mapping ## _len = 0;

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

static void * omap(const char * const fn, size_t * const len)
{
  int fd;
  struct stat st;
  void * const mapping = (fd = open(fn, O_RDONLY)) != -1 && ! fstat(fd, &st) ? mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0) : MAP_FAILED;
  close(fd);
  *len = mapping == MAP_FAILED ? 0 : st.st_size;
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

#define OMAP(suffix, mapping, depth) \
  sprintf(fne, "%s." # suffix, fn); \
  mapping = omap(fne, &mapping ## _len); \
  if (mapping == MAP_FAILED) \
    return unload_wl_s(depth), 1;

  OMAP(i, words_counts, 1)
  OMAP(s, strbase, 2)
  OMAP(c, charsbase, 3)
  OMAP(n, countsbase, 4)

  return 0;
}
