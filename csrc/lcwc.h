#ifndef LCWC_H
#define LCWC_H

#include <cstddef>
#include <vector>

#include "agram_types.h"
#include "wordlist.h"

unsigned long int wc_hash_chars(std::vector<agram_cpt> &);
unsigned long int wc_hash_chars(agram_cpt const *, unsigned int);

struct wc;

struct lc
{
  unsigned int len;
  unsigned int nchars;
  size_t str;
  size_t chars;
  unsigned long int hash;
};

struct wc
{
  unsigned int len;
  std::vector<agram_dchar> str;
  std::vector<agram_cpt> chars;
  std::vector<unsigned int> counts;
  unsigned long int hash;

  void sub_s(wordlist const *, wc const *, lc const *);
  wc(agram_dchar const *, size_t);
  bool is_anagram(wordlist const &, lc const &) const;
  bool contains(wordlist const &, lc const &) const;

  wc() : len(0), hash(0) {}
  void hash_chars() { hash = wc_hash_chars(chars); }
};

#endif
