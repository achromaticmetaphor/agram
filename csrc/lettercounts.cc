#define _POSIX_C_SOURCE 200809L

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <vector>

#include "agram_types.h"

#if AGRAM_ANDROID
#include "astr.h"
#endif

static void insert(const agram_cpt c, std::vector<agram_cpt> & l)
{
  for (auto e : l)
    if (e == c)
      return;
  l.push_back(c);
}

static void lettercounts_s(std::vector<unsigned int> & counts, std::vector<agram_cpt> & letters, const agram_cpt * const str, const size_t len)
{
  const agram_cpt * s;

  for (s = str; *s; s++)
    insert(*s, letters);
  std::sort(letters.begin(), letters.end());

  for (size_t i = 0; i < letters.size(); ++i)
    {
      counts.push_back(0);
      for (size_t j = 0; j < len; ++j)
        if (letters[i] == str[j])
          counts[i]++;
    }
}

void lettercounts(std::vector<unsigned int> & counts, std::vector<agram_cpt> & letters, const agram_dchar * const str, const size_t slen)
{
#if AGRAM_ANDROID
  agram_cpt tmp[slen + 1];
  lettercounts_s(counts, letters, tmp, uastrcpy(tmp, str, slen));
#else
  const size_t wlen = mbstowcs(nullptr, str, 0);
  agram_cpt tmp[wlen + 1];
  mbstowcs(tmp, str, wlen + 1);
  lettercounts_s(counts, letters, tmp, wlen);
#endif
}
