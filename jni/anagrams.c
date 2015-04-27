#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "agram_types.h"
#include "agram_wc.h"
#include "anagrams.h"
#include "is_within.h"
#include "lcwc.h"

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

size_t anagrams_single(struct agsto * const ostate)
{
top: ;
  struct agst * const state = ostate->states + ostate->depth;
  if (*state->wcsp)
    {
      memcpy(ostate->prefix + state->offset + 1, strbase + (**state->wcsp).str, sizeof(*ostate->prefix) * (**state->wcsp).len);
      if (state->target.len == (**state->wcsp).len)
        return state->offset + (**state->wcsp++).len;
      else
        {
          struct agst * const next_level = ostate->states + ostate->depth + 1;
          if (wc_sub(&next_level->target, &state->target, *state->wcsp))
            return (ostate->error = 1), 0;
          next_level->offset = state->offset + (**state->wcsp).len + 1;
#if AGRAM_ANDROID
          ostate->prefix[next_level->offset] = 32;
#else
          ostate->prefix[next_level->offset] = ' ';
#endif
          next_level->wcs_in = state->wcsp;
          next_level->wcs = state->wcs + state->wcslen + 1;
          next_level->wcsp = next_level->wcs;
          next_level->wcslen = filter_lc(next_level->wcs, next_level->wcs_in, &next_level->target);
          state->wcsp++;
          ostate->depth++;
          goto top;
        }
    }
  else
    {
      if (ostate->depth)
        {
          wc_free(&state->target);
          ostate->depth--;
          goto top;
        }
      else
        return 0;
    }
}

int anagrams_init(struct agsto * const ostate, agram_dchar const * const str, size_t const len)
{
  struct wc target;
  if (wc_init(&target, str, len))
    return 1;

  const size_t prefsize = target.len * 2 + 1;
  ostate->states = malloc(sizeof(*ostate->states) * prefsize);
  ostate->depth = 0;
  ostate->prefix = ostate->states ? malloc(sizeof(*ostate->prefix) * prefsize) : NULL;
  ostate->error = 0;
  if (! ostate->prefix)
    {
      wc_free(&target);
      anagrams_destroy(ostate);
      return 1;
    }

  struct agst * const state = ostate->states;
  state->target = target;
  state->offset = 0;
  state->wcs_in = alift(words_counts, NWORDS);
  state->wcs = state->wcs_in ? malloc(sizeof(*state->wcs) * (NWORDS + 1) * prefsize) : NULL;
  state->wcsp = state->wcs;
  state->wcslen = state->wcs ? filter_lc(state->wcs, state->wcs_in, &state->target) : 0;
  if (! state->wcs)
    return anagrams_destroy(ostate), 1;

  return 0;
}

void anagrams_destroy(struct agsto * const ostate)
{
  if (! ostate)
    return;
  for (; ostate->depth; ostate->depth--)
    wc_free(&ostate->states[ostate->depth].target);
  struct agst * const state = ostate->states;
  if (state)
    {
      wc_free(&state->target);
      free(state->wcs);
      free(state->wcs_in);
    }
  free(ostate->prefix);
  ostate->prefix = NULL;
  free(ostate->states);
  ostate->states = NULL;
}

int anagrams(agram_dchar const * const str, size_t const len, int (* const cb)(agram_dchar const *, size_t, void *), void * const cba)
{
  struct agsto ostate;
  if (anagrams_init(&ostate, str, len))
    return 1;
  int rval = 0;
  size_t slen;
  while (slen = anagrams_single(&ostate))
    if (cb(ostate.prefix + 1, slen, cba))
      {
        rval = 2;
        break;
      }
  rval |= ostate.error;
  anagrams_destroy(&ostate);
  return rval;
}
