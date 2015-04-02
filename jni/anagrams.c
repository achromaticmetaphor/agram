#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "agram_wc.h"
#include "astr.h"
#include "is_anagram.h"
#include "is_within.h"
#include "lcwc.h"

#include <jni.h>

static size_t filter_lc(const struct lc * * const out, const struct lc * const * wcs, const struct wc * const target)
{
  const struct lc * * outp = out;
  for (; *wcs; wcs++)
    if(is_within_lw(*wcs, target))
      *outp++ = *wcs;
  *outp = NULL;
  return outp - out;
}

static const struct lc * * alift(const struct lc * const wcs, const size_t len)
{
  size_t i;
  const struct lc * * const out = malloc(sizeof(*out) * (len+1));
  if (out)
    {
      for (i = 0; i < len; i++)
        out[i] = wcs+i;
      out[len] = NULL;
    }
  return out;
}

static int anagrams_print(const struct wc * const target, jchar * const prefix, const size_t offset, const struct lc * const * const wcs_in, const struct lc * * const wcs, int (* const cb)(jchar const *, size_t, void *), void * const cba)
{
  prefix[offset] = 32;
  const size_t wcslen = filter_lc(wcs, wcs_in, target);
  const struct lc * const * wcsp;
  for (wcsp = wcs; *wcsp; wcsp++)
    {
      memcpy(prefix+offset+1, strbase + (**wcsp).str, sizeof(*prefix) * (**wcsp).len);
      if (is_anagram(target, *wcsp))
        {
          if (cb(prefix + 1, offset + (**wcsp).len, cba))
            return 2;
        }
      else
        {
          struct wc new_target;
          if (wc_sub(&new_target, target, *wcsp))
            return 1;
          const int rr = anagrams_print(&new_target, prefix, offset + (**wcsp).len + 1, wcsp, wcs + wcslen + 1, cb, cba);
          if (rr)
            return rr;
          wc_free(&new_target);
        }
    }
  return 0;
}

int anagrams(jchar const * const str, size_t const len, int (* const cb)(jchar const *, size_t, void *), void * const cba)
{
  struct wc target;
  if (wc_init(&target, str, len))
    return 1;
  const size_t prefsize = target.len * 2 + 1;
  const struct lc * * const wcs = alift(words_counts, NWORDS);
  jchar * const prefix = wcs ? malloc(sizeof(*prefix) * prefsize) : NULL;
  const struct lc * * const scratch = prefix ? malloc(sizeof(*scratch) * (NWORDS + 1) * prefsize) : NULL;
  const int rval = scratch ? anagrams_print(&target, prefix, 0, wcs, scratch, cb, cba) : 1;
  free(scratch);
  free(wcs);
  free(prefix);
  wc_free(&target);
  return rval;
}
