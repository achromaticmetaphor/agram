#define _POSIX_C_SOURCE 200809L

#include <cstddef>
#include <cstdlib>
#include <map>
#include <vector>

#include "agram_types.h"

#if AGRAM_ANDROID
#include "astr.h"
#endif

static void lettercounts(std::vector<unsigned int> & counts,
                         std::vector<agram_cpt> & letters,
                         const agram_cpt * const str)
{
  std::map<agram_cpt, size_t> cmap;
  for (auto s = str; *s; s++)
    cmap[*s]++;

  for (auto & p : cmap)
    {
      letters.push_back(p.first);
      counts.push_back(p.second);
    }
}

void lettercounts(std::vector<unsigned int> & counts,
                  std::vector<agram_cpt> & letters,
                  const agram_dchar * const str, const size_t slen)
{
#if AGRAM_ANDROID
  std::vector<agram_cpt> tmp(slen + 1);
  uastrcpy(tmp.data(), str, slen);
  lettercounts(counts, letters, tmp.data());
#else
  const size_t wlen = mbstowcs(nullptr, str, 0);
  std::vector<agram_cpt> tmp(wlen + 1);
  mbstowcs(tmp.data(), str, wlen + 1);
  lettercounts(counts, letters, tmp.data());
#endif
}
