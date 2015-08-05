#include <stdlib.h>
#include <string.h>

#include "agram_types.h"
#include "agram_wc.h"
#include "astr.h"
#include "lcwc.h"
#include "lettercounts.h"

int wc_sub(struct wc * const out, const struct wc * const a, const struct lc * const b)
{
  out->str = NULL;
  out->len = a->len - b->len;
  out->chars = malloc(sizeof(*out->chars) * (a->nchars + 1));
  out->counts = malloc(sizeof(*out->counts) * (a->nchars + 1));
  if ((! out->chars) || (! out->counts))
    return wc_free(out), 1;
  {
    unsigned int a_i;
    unsigned int b_i = 0;
    unsigned int o_i = 0;
    for (a_i = 0; a_i < a->nchars; a_i++)
      if (a->chars[a_i] == charsbase[b->chars+b_i])
        {
          const unsigned int diff = a->counts[a_i] - countsbase[b->chars+b_i];
          if (diff)
            {
              out->chars[o_i] = a->chars[a_i];
              out->counts[o_i] = diff;
              o_i++;
            }
          b_i++;
        }
      else
        {
          out->chars[o_i] = a->chars[a_i];
          out->counts[o_i] = a->counts[a_i];
          o_i++;
        }
    out->chars[o_i] = 0;
    out->nchars = o_i;
  }
  return 0;
}

void wc_free(struct wc * const del)
{
  free((void *) del->str);
  free(del->chars);
  free(del->counts);
}

int wc_init(struct wc * const target, const agram_dchar * const str, const size_t slen)
{
  target->str = malloc(slen * sizeof(*target->str));
  if (! target->str)
    return 1;
  memcpy(target->str, str, slen * sizeof(*str));
  target->len = slen;
  target->chars = malloc(sizeof(*target->chars) * (target->len + 1));
  target->counts = malloc(sizeof(*target->counts) * (target->len + 1));
  if ((! target->chars) || (! target->counts))
    return wc_free(target), 1;
  target->nchars = lettercounts(target->counts, target->chars, str, slen);
  return 0;
}
