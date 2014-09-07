#include "lcwc.h"

int is_within(const struct wc * const a, const struct wc * const b)
{
  unsigned int i = 0, j = 0;
  while (i < a->nchars)
    {
      if (j >= b->nchars)
        return 0;

      if (a->chars[i] < b->chars[j])
        return 0;

      if (a->chars[i] == b->chars[j])
        {
          if (a->counts[i] > b->counts[j])
            return 0;
          else
            i++;
        }

      if (a->chars[i] > b->chars[j])
        j++;
    }

  return 1;
}
