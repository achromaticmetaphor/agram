#define _POSIX_C_SOURCE 200809L

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "agram_wc.h"
#include "agram_types.h"
#include "lcwc.h"

#define SUFFIX(combined, base, suffix) \
  char combined [strlen(base) + 3]; \
  sprintf(combined, "%s.%c", base, suffix);

struct lc_v2
{
  unsigned int len;
  unsigned int nchars;
  size_t str;
  size_t chars;
};

static int writeout(const char * const fn, void * const buff, size_t const bytes)
{
  FILE * const fp = fopen(fn, "w");
  int const write_succeeded = fp ? fwrite(buff, 1, bytes, fp) == bytes : 0;
  int const close_failed = fp ? fclose(fp) : 0;
  return (! write_succeeded) || close_failed;
}

static int duplicate(const char * const fnold, const char * const fnnew, const char suffix, void * buff, size_t bytes)
{
  SUFFIX(fnolds, fnold, suffix)
  SUFFIX(fnnews, fnnew, suffix)
  return link(fnolds, fnnews) && symlink(fnolds, fnnews) && writeout(fnnews, buff, bytes);
}

static int upgrade_lcs(const char * const fn)
{
  SUFFIX(fni, fn, 'i')
  const struct lc_v2 * const words_counts_v2 = (const struct lc_v2 *) words_counts;
  FILE * const fp = fopen(fni, "w");
  if (! fp)
    return 1;

  int failed = 0;
  agram_size i;
  for (i = 0; ! failed && i < NWORDS; i++)
    {
      const struct lc_v2 * const v2 = words_counts_v2 + i;
      struct lc const lcnew = {v2->len, v2->nchars, v2->str, v2->chars, wc_hash_chars(charsbase + v2->chars, v2->nchars)};
      failed = fwrite(&lcnew, sizeof(lcnew), 1, fp) != 1;
    }
  if (fclose(fp))
    failed = 1;
  return failed;
}

static int write_base(const char * const fn)
{
  FILE * const fp = fopen(fn, "w");
  if (! fp)
    return 1;

  const long tells[4] = {sizeof(*words_counts) * NWORDS, strbase_len, charsbase_len, countsbase_len};
  int const write_failed = fwrite(&NWORDS, sizeof(NWORDS), 1, fp) != 1 || fwrite(tells, sizeof(*tells), 4, fp) != 4;
  int const close_failed = fclose(fp);
  return write_failed || close_failed;
}

int agram_wl_can_upgrade(char const version)
{
  return version == 2;
}

int agram_wl_upgrade(const char * const fnold, const char * const fnnew, char const version)
{
  int const error = version != 2
    || load_wl(fnold)
    || duplicate(fnold, fnnew, 's', strbase, strbase_len)
    || duplicate(fnold, fnnew, 'c', charsbase, charsbase_len)
    || duplicate(fnold, fnnew, 'n', countsbase, countsbase_len)
    || upgrade_lcs(fnnew)
    || write_base(fnnew);
  unload_wl();
  return error;
}
