#include <cstddef>
#include <cstdlib>
#include <map>
#include <vector>

#include "agram_types.h"

#if AGRAM_ANDROID
#include "astr.h"
#endif

static void lettercounts(std::vector<unsigned int> & counts,
                         std::vector<agram_codepoint> & letters)
{
  std::map<agram_codepoint, size_t> cmap;
  for (auto c : letters)
    ++cmap[c];
  letters.clear();

  for (auto & p : cmap)
    {
      letters.push_back(p.first);
      counts.push_back(p.second);
    }
}

void lettercounts(std::vector<unsigned int> & counts,
                  std::vector<agram_codepoint> & letters,
                  const agram_display_char * const str, const size_t slen)
{
#if AGRAM_ANDROID
  letters.resize(slen + 1);
  uastrcpy(letters.data(), str, slen);
#else
  letters.resize(mbstowcs(nullptr, str, 0) + 1);
  mbstowcs(letters.data(), str, letters.size());
#endif
  letters.pop_back();
  lettercounts(counts, letters);
}
