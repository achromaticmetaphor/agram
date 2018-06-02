#define _POSIX_C_SOURCE 200809L

#include <vector>

#include "agram_types.h"
#include "lcwc.h"
#include "lettercounts.h"
#include "wordlist.h"

unsigned long int wc_hash_chars(agram_cpt const * const chars,
                                unsigned int const nchars)
{
  unsigned long int h = 0;
  unsigned int i;
  for (i = 0; i < nchars; i++)
    h |= 1 << (chars[i] % 32);
  return h;
}

unsigned long int wc_hash_chars(std::vector<agram_cpt> & chars)
{
  return wc_hash_chars(chars.data(), chars.size());
}

void wc::sub_s(const struct wordlist * const wl, const struct wc * const a,
               const struct lc * const b)
{
  len = a->len - b->len;
  chars.clear();
  counts.clear();

  unsigned int b_i = 0;
  for (unsigned int a_i = 0; a_i < a->chars.size(); a_i++)
    if (b_i < b->nchars && a->chars[a_i] == wl->charsbase[b->chars + b_i])
      {
        const unsigned int diff =
            a->counts[a_i] - wl->countsbase[b->chars + b_i];
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

wc::wc(const agram_dchar * const sstr, const size_t slen) : len(slen), hash(0)
{
  for (size_t i = 0; i < slen; ++i)
    str.push_back(sstr[i]);
  lettercounts(counts, chars, sstr, slen);
  hash_chars();
}

bool wc::is_anagram(wordlist const & wl, lc const & b) const
{
  if (len != b.len)
    return false;
  if (chars.size() != b.nchars)
    return false;
  for (unsigned int i = 0; i < b.nchars; ++i)
    {
      if (chars[i] != wl.charsbase[b.chars + i])
        return false;
      if (counts[i] != wl.countsbase[b.chars + i])
        return false;
    }
  return true;
}

bool wc::contains(wordlist const & wl, lc const & a) const
{
  if ((a.hash & hash) != a.hash)
    return false;
  unsigned int i = 0, j = 0;
  while (i < a.nchars)
    {
      if (j >= chars.size())
        return false;

      if (wl.charsbase[a.chars + i] < chars[j])
        return false;

      else if (wl.charsbase[a.chars + i] == chars[j])
        if (wl.countsbase[a.chars + i] > counts[j])
          return false;
        else
          ++i;

      else if (wl.charsbase[a.chars + i] > chars[j])
        ++j;
    }

  return true;
}
