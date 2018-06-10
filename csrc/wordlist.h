#ifndef WORDLIST_H
#define WORDLIST_H

#include <vector>

#include "agram_types.h"
#include "string_view.h"
#include "wordlist_entry.h"

struct wordlist
{
  std::vector<wordlist_entry> words_counts;
  std::vector<agram_display_char> strbase;
  std::vector<agram_codepoint> charsbase;
  std::vector<unsigned int> countsbase;

  std::size_t size() const { return words_counts.size(); }

  string_view<agram_display_char>
  display_string(wordlist_entry const & entry) const
  {
    return {strbase.data() + entry.str, entry.len};
  }

  string_view<agram_display_char> display_string(std::size_t const i) const
  {
    return display_string(words_counts[i]);
  }
};

#endif
