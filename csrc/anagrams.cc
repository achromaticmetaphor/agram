#include <cstddef>
#include <functional>
#include <vector>

#include "agram_types.h"
#include "anagrams.h"
#include "optional.h"
#include "string_view.h"
#include "wordlist.h"
#include "wordlist_entry.h"

static void filter_entries(
    wordlist const & wl,
    std::vector<std::reference_wrapper<wordlist_entry const>> & out,
    std::vector<std::reference_wrapper<wordlist_entry const>> const & in,
    size_t const skip, anagram_target const & target)
{
  for (auto it = in.begin() + skip; it != in.end(); ++it)
    if (target.contains(wl, *it))
      out.push_back(*it);
}

static void filter_entries(
    wordlist const & wl,
    std::vector<std::reference_wrapper<wordlist_entry const>> & out,
    anagram_target const & target)
{
  for (auto & entry : wl.words_counts)
    if (target.contains(wl, entry))
      out.push_back(entry);
}

optional<string_view<agram_display_char>> agsto::single()
{
  while (true)
    {
      auto & state = states[depth];
      if (state.exhausted())
        if (depth == 0)
          return {};
        else
          --depth;
      else
        {
          for (unsigned int i = 0; i < state.current().len; ++i)
            prefix[state.offset + 1 + i] =
                wl.strbase[state.current().str + i];
          if (state.target.len == state.current().len)
            return {prefix.data() + 1, state.offset + state.advance().len};
          else
            {
              auto & next_level = states[++depth];
              next_level.target.sub_s(&wl, &state.target, state.current());
              next_level.offset = state.offset + state.current().len + 1;
#if AGRAM_ANDROID
              prefix[next_level.offset] = 32;
#else
              prefix[next_level.offset] = ' ';
#endif
              next_level.wcs.clear();
              next_level.wcsi = 0;
              filter_entries(wl, next_level.wcs, state.wcs, state.wcsi,
                             next_level.target);
              state.advance();
            }
        }
    }
}

agsto::agsto(wordlist const & wl, string_view<agram_display_char> sv)
    : depth(0), wl(wl)
{
  anagram_target target(sv);
  size_t const prefsize = target.len * 2 + 1;
  states.resize(prefsize);
  prefix.resize(prefsize);
  states[0].target = target;
  states[0].offset = 0;
  states[0].wcsi = 0;
  filter_entries(wl, states[0].wcs, target);
}
