#include "agram_wc.h"
#include "lcwc.h"

int is_within_lw(const struct lc * const a, const struct wc * const b)
{
  agram_cpt * const a_chars = a->chars + charsbase;
  unsigned int * const a_counts = a->chars + countsbase;
  unsigned int i = 0, j = 0;
  while (i < a->nchars)
    {
      if (j >= b->nchars)
        return 0;

      if (a_chars[i] < b->chars[j])
        return 0;

      if (a_chars[i] == b->chars[j])
        {
          if (a_counts[i] > b->counts[j])
            return 0;
          else
            i++;
        }

      if (a_chars[i] > b->chars[j])
        j++;
    }

  return 1;
}
