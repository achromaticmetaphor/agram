#define _POSIX_C_SOURCE 200809L

#include <cstddef>
#include <functional>
#include <vector>

#include "agram_types.h"
#include "anagrams.h"
#include "lcwc.h"
#include "wordlist.h"

static void filter_lc(wordlist const & wl, std::vector<std::reference_wrapper<lc const>> & out, std::vector<std::reference_wrapper<lc const>> const & in, size_t const skip, wc const & target)
{
  for (auto it = in.begin() + skip; it != in.end(); ++it)
    if (target.contains(wl, *it))
      out.push_back(*it);
}

static void filter_lc(wordlist const & wl, std::vector<std::reference_wrapper<lc const>> & out, wc const & target)
{
  for (auto & lc : wl.words_counts)
    if (target.contains(wl, lc))
      out.push_back(lc);
}

size_t agsto::single()
{
  while (true)
    {
      auto & state = states[depth];
      if (state.exhausted())
        if (depth == 0)
          return 0;
        else
          --depth;
      else
        {
          for (unsigned int i = 0; i < state.current().len; ++i)
            prefix[state.offset + 1 + i] = wl.strbase[state.current().str + i];
          if (state.target.len == state.current().len)
            return state.offset + state.advance().len;
          else
            {
              auto & next_level = states[++depth];
              next_level.target.sub_s(&wl, &state.target, &state.current());
              next_level.offset = state.offset + state.current().len + 1;
#if AGRAM_ANDROID
              prefix[next_level.offset] = 32;
#else
              prefix[next_level.offset] = ' ';
#endif
              next_level.wcs.clear();
              next_level.wcsi = 0;
              filter_lc(wl, next_level.wcs, state.wcs, state.wcsi, next_level.target);
              state.advance();
            }
        }
    }
}

agsto::agsto(wordlist const & wl, agram_dchar const * const str, size_t const len) : depth(0), wl(wl)
{
  struct wc target(str, len);
  size_t const prefsize = target.len * 2 + 1;
  states.resize(prefsize);
  prefix.resize(prefsize);
  states[0].target = target;
  states[0].offset = 0;
  states[0].wcsi = 0;
  filter_lc(wl, states[0].wcs, target);
}
