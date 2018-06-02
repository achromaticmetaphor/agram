#define _POSIX_C_SOURCE 200809L

#include <clocale>
#include <cstring>
#include <iostream>
#include <random>

#include "anagram.h"
#include "anagrams.h"
#include "wordlist.h"
#include "words_from.h"
#include "xdgwl.h"

static int prn(const agram_dchar * str, size_t const len)
{
  for (size_t i = 0; i < len; ++i)
    std::cout << str[i];
  std::cout << std::endl;
  return 0;
}

static int usage(const char * pn)
{
  std::cerr << "usage: " << pn << " <single|multi|random|contained> <string>"
            << std::endl;
  return 1;
}

static int single(wordlist const & wl, int argc, char * argv[])
{
  if (argc == 3)
    return anagram(wl, argv[2], strlen(argv[2]), prn);
  return usage(argv[0]);
}

static int multi(wordlist const & wl, int argc, char * argv[])
{
  if (argc == 3)
    return anagrams(wl, argv[2], strlen(argv[2]), prn);
  return usage(argv[0]);
}

static int shuffled(wordlist const & wl, int argc, char * argv[])
{
  const long int count = argc >= 3 ? atol(argv[2]) : 1;
  std::random_device rd;
  std::default_random_engine dre(rd());
  std::uniform_int_distribution<agram_size> dist(0, wl.words_counts.size());
  for (long int i = 0; i < count; ++i)
    {
      agram_size const n = dist(dre);
      prn(wl.words_counts[n].str + wl.strbase.data(), wl.words_counts[n].len);
    }
  return 0;
}

static int contained(wordlist const & wl, int argc, char * argv[])
{
  if (argc >= 3)
    return words_from(wl, argv[2], strlen(argv[2]), argc >= 4, prn);
  return usage(argv[0]);
}

int main(int argc, char * argv[])
{
  setlocale(LC_ALL, "");

  struct wordlist wl;
  if (init_wl(wl))
    return 1;
  if (argc < 2)
    return usage(argv[0]);

  switch (argv[1][0])
    {
      case 's':
        return single(wl, argc, argv);
      case 'm':
        return multi(wl, argc, argv);
      case 'r':
        return shuffled(wl, argc, argv);
      case 'c':
        return contained(wl, argc, argv);
      default:
        return usage(argv[0]);
    }
}
