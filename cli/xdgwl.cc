#define _POSIX_C_SOURCE 200809L

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>

#include "wc.h"
#include "wordlist.h"

static std::string find_wordlist(void)
{
  char const * const override_wl = getenv("AGRAM_WORDLIST");
  if (override_wl)
    return override_wl;

  std::vector<std::string> wordlist_filenames = {"agram/words", "dict/words"};
  for (std::string const & name : wordlist_filenames)
    {
#define RETURN_IF_READABLE(b, p)                                    \
  do                                                                \
    {                                                               \
      std::string wordlist = std::string(b) + "/" + p + "/" + name; \
      if (!access(wordlist.c_str(), F_OK))                          \
        return wordlist;                                            \
    }                                                               \
  while (0)

      char const * const xdg_data_home = getenv("XDG_DATA_HOME");
      if (xdg_data_home && xdg_data_home[0])
        RETURN_IF_READABLE(xdg_data_home, "");
      else
        {
          char const * home = getenv("HOME");
          if (!home)
            home = "";
          RETURN_IF_READABLE(home, ".local/share");
        }

      char const * xdg_data_dirs = getenv("XDG_DATA_DIRS");
      if (!xdg_data_dirs || !xdg_data_dirs[0])
        xdg_data_dirs = "/usr/local/share/:/usr/share/";

      char xdg_data_dirs_m[strlen(xdg_data_dirs) + 1];
      strcpy(xdg_data_dirs_m, xdg_data_dirs);
      for (char * data_dir = strtok(xdg_data_dirs_m, ":"); data_dir; data_dir = strtok(NULL, ":"))
        RETURN_IF_READABLE(data_dir, "");
    }

  return "";
}

struct src : cwlsrc
{
  std::ifstream wordlist;
  std::string next;

  int has_next()
  {
    std::getline(wordlist, next);
    return wordlist.good();
  }

  size_t len() { return next.size(); }
  void get(agram_dchar * const out) { strcpy(out, next.data()); }

  src(std::string & fn) : wordlist(fn) {}
};

int init_wl(wordlist & wl)
{
  std::string wordlist = find_wordlist();
  if (wordlist.empty())
    {
      std::cerr << "error: No wordlist was found." << std::endl;
      std::cerr << "Wordlists are searched for in $XDG_DATA_DIRS/agram/words and $XDG_DATA_DIRS/dict/words." << std::endl;
      std::cerr << "A wordlist may also be specified in AGRAM_WORDLIST." << std::endl;
      return 1;
    }
  return src(wordlist).build_wl(&wl);
}
