#include "lcwc.h"
#include "wordlist.h"

int is_within_lw(const struct wordlist * const wl, const struct lc * const a, const struct wc * const b)
{
  if ((a->hash & b->hash) != a->hash)
    return 0;
  agram_cpt * const a_chars = a->chars + wl->charsbase;
  unsigned int * const a_counts = a->chars + wl->countsbase;
  unsigned int i = 0, j = 0;
  while (i < a->nchars)
    {
      if (j >= b->nchars)
        return 0;

      if (a_chars[i] < b->chars[j])
        return 0;

      else if (a_chars[i] == b->chars[j])
        {
          if (a_counts[i] > b->counts[j])
            return 0;
          else
            i++;
        }

      else if (a_chars[i] > b->chars[j])
        j++;
    }

  return 1;
}
