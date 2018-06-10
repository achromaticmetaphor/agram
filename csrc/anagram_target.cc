#include <algorithm>
#include <vector>

#include "agram_types.h"
#include "anagram_target.h"
#include "lettercounts.h"
#include "wordlist.h"
#include "wordlist_entry.h"

void anagram_target::sub_s(const wordlist * const wl,
                           const anagram_target * const a,
                           const wordlist_entry & b)
{
  len = a->len - b.len;
  chars.clear();
  counts.clear();

  unsigned int b_i = 0;
  for (unsigned int a_i = 0; a_i < a->chars.size(); a_i++)
    if (b_i < b.nchars && a->chars[a_i] == wl->charsbase[b.chars + b_i])
      {
        const unsigned int diff =
            a->counts[a_i] - wl->countsbase[b.chars + b_i];
        if (diff)
          {
            chars.push_back(a->chars[a_i]);
            counts.push_back(diff);
          }
        b_i++;
      }
    else
      {
        chars.push_back(a->chars[a_i]);
        counts.push_back(a->counts[a_i]);
      }
  hash_chars();
}

anagram_target::anagram_target(string_view<agram_display_char> sv)
    : len(sv.len), hash(0)
{
  for (size_t i = 0; i < sv.len; ++i)
    str.push_back(sv.data[i]);
  lettercounts(counts, chars, sv.data, sv.len);
  hash_chars();
}

bool anagram_target::is_anagram(wordlist const & wl,
                                wordlist_entry const & b) const
{
  return len == b.len && chars.size() == b.nchars &&
         std::equal(chars.cbegin(), chars.cend(),
                    wl.charsbase.cbegin() + b.chars) &&
         std::equal(counts.cbegin(), counts.cend(),
                    wl.countsbase.cbegin() + b.chars);
}

bool anagram_target::contains(wordlist const & wl,
                              wordlist_entry const & a) const
{
  if ((a.hash & hash) != a.hash)
    return false;

  std::size_t j = 0;
  for (std::size_t i = 0; i < a.nchars; ++i)
    {
      while (j < chars.size() && chars[j] < wl.charsbase[a.chars + i])
        ++j;
      if (j >= chars.size() || chars[j] != wl.charsbase[a.chars + i] ||
          counts[j] < wl.countsbase[a.chars + i])
        return false;
    }

  return true;
}
