#include <cstddef>
#include <cstdlib>
#include <map>
#include <vector>

#include "agram_types.h"

#if AGRAM_ANDROID
static inline void uastrcpy(std::vector<agram_codepoint> & dest,
                            const agram_display_char * const src,
                            const size_t slen)
{
  dest.clear();
  for (std::size_t i = 0; i < slen; ++i)
    if (src[i] >= 0xd800 && src[i] < 0xe000 && i + 1 < slen)
      {
        dest.push_back(((src[i] - 0xd800) << 10) | (src[i + 1] - 0xdc00));
        i++;
      }
    else
      dest.push_back(src[i]);
}
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
  uastrcpy(letters, str, slen);
#else
  letters.resize(mbstowcs(nullptr, str, 0) + 1);
  mbstowcs(letters.data(), str, letters.size());
  letters.pop_back();
#endif
  lettercounts(counts, letters);
}
