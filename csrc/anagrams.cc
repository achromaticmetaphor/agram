#define _POSIX_C_SOURCE 200809L

#include <cstddef>
#include <cstdlib>
#include <cstring>

#include "agram_types.h"
#include "anagrams.h"
#include "is_within.h"
#include "lcwc.h"
#include "wordlist.h"

#define FILTER_FUN(FUN, POINTER, DEREF)                                                                                                                                       \
  static size_t FUN(const struct wordlist * const wl, const struct lc ** const out, const struct lc * const POINTER wcs, const struct wc * const target, const size_t wcslen) \
  {                                                                                                                                                                           \
    const struct lc ** outp = out;                                                                                                                                            \
    size_t i;                                                                                                                                                                 \
    for (i = 0; i < wcslen; i++)                                                                                                                                              \
      if (is_within_lw(wl, DEREF(wcs + i), target))                                                                                                                           \
        *outp++ = DEREF(wcs + i);                                                                                                                                             \
    *outp = NULL;                                                                                                                                                             \
    return outp - out;                                                                                                                                                        \
  }

FILTER_FUN(filter_lc, *const, *)
FILTER_FUN(filter_lc_alift, , )
#undef FILTER_FUN

size_t anagrams_single(struct agsto * const ostate)
{
top:;
  struct agst * const state = ostate->states + ostate->depth;
  if (*state->wcsp)
    {
      memcpy(ostate->prefix + state->offset + 1, ostate->wl->strbase + (**state->wcsp).str, sizeof(*ostate->prefix) * (**state->wcsp).len);
      if (state->target.len == (**state->wcsp).len)
        return state->offset + (**state->wcsp++).len;
      else
        {
          struct agst * const next_level = ostate->states + ostate->depth + 1;
          next_level->target.sub_s(ostate->wl, &state->target, *state->wcsp);
          next_level->offset = state->offset + (**state->wcsp).len + 1;
#if AGRAM_ANDROID
          ostate->prefix[next_level->offset] = 32;
#else
          ostate->prefix[next_level->offset] = ' ';
#endif
          next_level->wcs = state->wcs + state->wcslen + 1;
          next_level->wcsp = next_level->wcs;
          next_level->wcslen = filter_lc(ostate->wl, next_level->wcs, state->wcsp, &next_level->target, state->wcslen - (state->wcsp - state->wcs));
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

static void agsto_free(struct agsto * const ostate)
{
#define FREE_AND_CLEAR(lval) \
  do                         \
    {                        \
      if (lval)              \
        delete[] lval;       \
      lval = NULL;           \
    }                        \
  while (0)
  FREE_AND_CLEAR(ostate->prefix);
  FREE_AND_CLEAR(ostate->states);
#undef FREE_AND_CLEAR
}

static int agsto_alloc(struct agsto * const ostate, size_t const len, size_t const nchars, size_t const nwords)
{
  size_t const prefsize = len * 2 + 1;
  ostate->states = new agst[prefsize];
  ostate->prefix = new agram_dchar[prefsize];
  ostate->states[0].wcs = new lc const *[(nwords + 1) * prefsize];
  return 0;
}

int anagrams_init(struct agsto * const ostate, const struct wordlist * const wl, agram_dchar const * const str, size_t const len)
{
  struct wc target(str, len);
  if (agsto_alloc(ostate, target.len, target.chars.size(), wl->nwords))
    return 1;

  ostate->wl = wl;
  ostate->depth = 0;
  ostate->states[0].target = target;
  ostate->states[0].offset = 0;
  ostate->states[0].wcsp = ostate->states[0].wcs;
  ostate->states[0].wcslen = filter_lc_alift(wl, ostate->states[0].wcs, wl->words_counts, &target, wl->nwords);
  return 0;
}

void anagrams_destroy(struct agsto * const ostate)
{
  if (!ostate)
    return;
  ostate->depth = 0;
  struct agst * const state = ostate->states;
  if (state)
    delete[] state->wcs;
  agsto_free(ostate);
}

int anagrams(const struct wordlist * const wl, agram_dchar const * const str, size_t const len, int (*const cb)(agram_dchar const *, size_t, void *), void * const cba)
{
  struct agsto ostate;
  if (anagrams_init(&ostate, wl, str, len))
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
