#include <clocale>
#include <cstring>
#include <iostream>
#include <random>

#include "anagram.h"
#include "anagrams.h"
#include "string_view.h"
#include "wordlist.h"
#include "words_from.h"
#include "xdgwl.h"

static int print(string_view<char> sv)
{
  std::cout << sv << std::endl;
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
    return anagram(wl, argv[2], print);
  return usage(argv[0]);
}

static int multi(wordlist const & wl, int argc, char * argv[])
{
  if (argc == 3)
    return anagrams(wl, argv[2], print);
  return usage(argv[0]);
}

static int shuffled(wordlist const & wl, int argc, char * argv[])
{
  const long int count = argc >= 3 ? atol(argv[2]) : 1;
  std::random_device rd;
  std::default_random_engine dre(rd());
  std::uniform_int_distribution<agram_size> dist(0, wl.size());
  for (long int i = 0; i < count; ++i)
    {
      agram_size const n = dist(dre);
      print(wl.display_string(n));
    }
  return 0;
}

static int contained(wordlist const & wl, int argc, char * argv[])
{
  if (argc >= 3)
    return words_from(wl, argv[2], argc >= 4, print);
  return usage(argv[0]);
}

int main(int argc, char * argv[])
{
  setlocale(LC_ALL, "");

  wordlist wl;
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
