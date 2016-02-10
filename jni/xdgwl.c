#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wc.h"
#include "wordlist.h"

static FILE * open_path(const char * const base, const char * const path, const char * const file, const char * const mode)
{
  char filepath[strlen(base) + strlen(path) + strlen(file) + 3];
  sprintf(filepath, "%s/%s/%s", base, path, file);
  return fopen(filepath, mode);
}

static FILE * find_wordlist(void)
{
  char * const override_wl = getenv("AGRAM_WORDLIST");
  if (override_wl)
    return fopen(override_wl, "r");

  const char * const wordlist_filenames[] = {"agram/words", "dict/words", NULL};
  for (int i = 0; wordlist_filenames[i]; i++)
    {
#define RETURN_IF_READABLE(b, p) \
  do { \
    FILE * const wordlist = open_path(b, p, wordlist_filenames[i], "r"); \
    if (wordlist) \
      return wordlist; \
  } while (0)

      char * xdg_data_home = getenv("XDG_DATA_HOME");
      if (xdg_data_home && xdg_data_home[0])
        RETURN_IF_READABLE(xdg_data_home, "");
      else
        {
          char * home = getenv("HOME");
          if (! home)
            home = "";
          RETURN_IF_READABLE(home, ".local/share");
        }

      char * xdg_data_dirs = getenv("XDG_DATA_DIRS");
      if (! xdg_data_dirs || ! xdg_data_dirs[0])
        xdg_data_dirs = "/usr/local/share/:/usr/share/";

      char xdg_data_dirs_m[strlen(xdg_data_dirs) + 1];
      strcpy(xdg_data_dirs_m, xdg_data_dirs);
      for (char * data_dir = strtok(xdg_data_dirs_m, ":"); data_dir; data_dir = strtok(NULL, ":"))
        RETURN_IF_READABLE(data_dir, "");
    }

  return NULL;
}

struct cba
{
  FILE * wordlist;
  char next [1024];
};

static int has_next(void * vcba)
{
  struct cba * cba = vcba;
  if (fgets(cba->next, 1024, cba->wordlist) == NULL)
    return 0;
  char * const newline = strchr(cba->next, '\n');
  if (! newline || newline == cba->next)
    return 0;
  *newline = 0;
  return 1;
}

static size_t len(void * vcba)
{
  return strlen(((struct cba *) vcba)->next);
}

static void get(agram_dchar * out, void * vcba)
{
  strcpy(out, ((struct cba *) vcba)->next);
}

int init_wl(struct wordlist * const wl)
{
  static const struct cwlcbs cbs = {has_next, len, get};
  struct cba cba;

  cba.wordlist = find_wordlist();
  if (! cba.wordlist) {
    fprintf(stderr, "error: No wordlist was found.\n");
    fprintf(stderr, "Wordlists are searched for in $XDG_DATA_DIRS/agram/words and $XDG_DATA_DIRS/dict/words.\n");
    fprintf(stderr, "A wordlist may also be specified in AGRAM_WORDLIST.\n");
    return 1;
  }

  return build_wl(wl, &cbs, &cba);
}
