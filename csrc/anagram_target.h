#ifndef ANAGRAM_TARGET_H
#define ANAGRAM_TARGET_H

#include <cstddef>
#include <numeric>
#include <vector>

#include "agram_types.h"
#include "wordlist.h"
#include "wordlist_entry.h"

static inline unsigned long int wc_hash_chars(std::vector<agram_cpt> & chars)
{
  return std::accumulate(
      chars.begin(), chars.end(), 0ul,
      [](unsigned long int a, agram_cpt b) { return a | (1 << (b % 32)); });
}

struct anagram_target
{
  unsigned int len;
  std::vector<agram_dchar> str;
  std::vector<agram_cpt> chars;
  std::vector<unsigned int> counts;
  unsigned long int hash;

  void sub_s(wordlist const *, anagram_target const *,
             wordlist_entry const &);
  anagram_target(agram_dchar const *, size_t);
  bool is_anagram(wordlist const &, wordlist_entry const &) const;
  bool contains(wordlist const &, wordlist_entry const &) const;

  anagram_target() : len(0), hash(0) {}
  void hash_chars() { hash = wc_hash_chars(chars); }
};

#endif
