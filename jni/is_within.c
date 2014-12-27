#include "lcwc.h"

static int is_within(const struct wc * const a, const struct wc * const b)
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

int is_within_wl(const struct wc * const a, const struct lc * const b)
{
  struct wc tmp;
  lcwc(&tmp, b);
  return is_within(a, &tmp);
}

int is_within_lw(const struct lc * const a, const struct wc * const b)
{
  struct wc tmp;
  lcwc(&tmp, a);
  return is_within(&tmp, b);
}
