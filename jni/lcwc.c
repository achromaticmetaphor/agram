#include <stdlib.h>
#include <string.h>

#include "agram_wc.h"
#include "astr.h"
#include "lcwc.h"
#include "lettercounts.h"

#include <jni.h>

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

int wc_init (struct wc * const target, const jint * const str)
{
  target->str = astrdup(str);
  if (! target->str)
    return 1;
  target->len = astrlen(str);
  target->chars = malloc(sizeof(*target->chars) * (target->len + 1));
  target->counts = malloc(sizeof(*target->counts) * (target->len + 1));
  if ((! target->chars) || (! target->counts))
    return wc_free(target), 1;
  lettercounts(target->counts, target->chars, str);
  target->nchars = astrlen(target->chars);
  return 0;
}

void lcwc(struct wc * const a, const struct lc * const b)
{
  a->len = b->len;
  a->nchars = b->nchars;
  a->str = strbase + b->str;
  a->chars = charsbase + b->chars;
  a->counts = countsbase + b->chars;
}
