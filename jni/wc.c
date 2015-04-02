#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "agram_wc.h"
#include "astr.h"
#include "lcwc.h"
#include "lettercounts.h"
#include "wc.h"

static int putout(const int fd, const void * buf, size_t len)
{
  while (len)
    {
      const ssize_t written = write(fd, buf, len);
      if (written == -1)
        if (errno == EINTR)
          continue;
        else
          return 1;
      len -= written;
      buf = (char *) buf + written;
    }
  return 0;
}

int compile_wl(const char * const outfn, struct cwlcbs const * const cbs, void * const cba)
{
  char fne[strlen(outfn)+3];
  sprintf(fne, "%s.i", outfn);
  const int fdi = creat(fne, S_IRUSR);
  if (fdi == -1)
    return 1;

  sprintf(fne, "%s.s", outfn);
  const int fds = creat(fne, S_IRUSR);
  if (fds == -1)
    goto fail_i;

  sprintf(fne, "%s.c", outfn);
  const int fdc = creat(fne, S_IRUSR);
  if (fdc == -1)
    goto fail_s;

  sprintf(fne, "%s.n", outfn);
  const int fdn = creat(fne, S_IRUSR);
  if (fdn == -1)
    goto fail_c;

  agram_size NWORDS = 0;
  size_t stroff = 0;
  size_t charsoff = 0;
  while (cbs->has_next(cba))
    {
      struct lc index;
      NWORDS++;
      index.len = cbs->len(cba);
      agram_dchar str[index.len];
      unsigned int counts[index.len];
      agram_cpt chars[index.len];
      cbs->get(str, cba);
      index.nchars = lettercounts(counts, chars, str, index.len);
      index.str = stroff;
      index.chars = charsoff;
      stroff += index.len;
      charsoff += index.nchars;

      if (putout(fdi, &index, sizeof(index))
       || putout(fds, str, sizeof(str))
       || putout(fdc, chars, sizeof(*chars) * index.nchars)
       || putout(fdn, counts, sizeof(*counts) * index.nchars))
        goto fail_n;
    }

  if (fsync(fdn) | close(fdn)
    | fsync(fdc) | close(fdc)
    | fsync(fds) | close(fds)
    | fsync(fdi) | close(fdi))
    return 1;

  const int fd = creat(outfn, S_IRUSR);
  if (putout(fd, &NWORDS, sizeof(NWORDS)) | fsync(fd) | close(fd))
    return 1;

  sprintf(fne, "%s.k", outfn);
  const int fdk = creat(fne, S_IRUSR);
  fsync(fdk);
  close(fdk);

  return 0;

fail_n:
  close(fdn);
fail_c:
  close(fdc);
fail_s:
  close(fds);
fail_i:
  close(fdi);
  return 1;
}
