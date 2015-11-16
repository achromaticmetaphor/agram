#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "agram_types.h"
#include "agram_wc.h"
#include "anagrams.h"
#include "is_within.h"
#include "lcwc.h"

#define FILTER_FUN(FUN, POINTER, DEREF) \
  static size_t FUN (const struct lc * * const out, const struct lc * const POINTER wcs, const struct wc * const target, const size_t wcslen) \
  { \
    const struct lc * * outp = out; \
    size_t i; \
    for (i = 0; i < wcslen; i++) \
      if(is_within_lw(DEREF (wcs + i), target)) \
        *outp++ = DEREF (wcs + i); \
    *outp = NULL; \
    return outp - out; \
  }

FILTER_FUN(filter_lc, * const, *)
FILTER_FUN(filter_lc_alift, , )
#undef FILTER_FUN

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
          size_t const scratch_offset = (ostate->depth + 1) * (ostate->states[0].target.nchars + 1);
          next_level->target.chars = ostate->chars_scratch + scratch_offset;
          next_level->target.counts = ostate->counts_scratch + scratch_offset;
          wc_sub_s(&next_level->target, &state->target, *state->wcsp);
          next_level->offset = state->offset + (**state->wcsp).len + 1;
#if AGRAM_ANDROID
          ostate->prefix[next_level->offset] = 32;
#else
          ostate->prefix[next_level->offset] = ' ';
#endif
          next_level->wcs = state->wcs + state->wcslen + 1;
          next_level->wcsp = next_level->wcs;
          next_level->wcslen = filter_lc(next_level->wcs, state->wcsp, &next_level->target, state->wcslen - (state->wcsp - state->wcs));
          state->wcsp++;
          ostate->depth++;
          goto top;
        }
    }
  else
    {
      if (ostate->depth)
        {
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
  size_t const scratch_len = target.nchars + 1;
  ostate->states = malloc(sizeof(*ostate->states) * prefsize);
  ostate->depth = 0;
  ostate->prefix = ostate->states ? malloc(sizeof(*ostate->prefix) * prefsize) : NULL;
  ostate->chars_scratch = ostate->prefix ? malloc(sizeof(*ostate->chars_scratch) * scratch_len * prefsize) : NULL;
  ostate->counts_scratch = ostate->chars_scratch ? malloc(sizeof(*ostate->counts_scratch) * scratch_len * prefsize) : NULL;
  if (! ostate->counts_scratch)
    {
      wc_free(&target);
      anagrams_destroy(ostate);
      return 1;
    }

  struct agst * const state = ostate->states;
  state->target = target;
  state->offset = 0;
  state->wcs = malloc(sizeof(*state->wcs) * (NWORDS + 1) * prefsize);
  state->wcsp = state->wcs;
  state->wcslen = state->wcs ? filter_lc_alift(state->wcs, words_counts, &state->target, NWORDS) : 0;
  if (! state->wcs)
    return anagrams_destroy(ostate), 1;

  return 0;
}

void anagrams_destroy(struct agsto * const ostate)
{
  if (! ostate)
    return;
  ostate->depth = 0;
  struct agst * const state = ostate->states;
  if (state)
    {
      wc_free(&state->target);
      free(state->wcs);
    }
#define FREE_AND_CLEAR(lval) do { free(lval); lval = NULL; } while (0)
  FREE_AND_CLEAR(ostate->prefix);
  FREE_AND_CLEAR(ostate->states);
  FREE_AND_CLEAR(ostate->chars_scratch);
  FREE_AND_CLEAR(ostate->counts_scratch);
#undef FREE_AND_CLEAR
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
  anagrams_destroy(&ostate);
  return rval;
}
