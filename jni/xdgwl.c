#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "agram_wc.h"
#include "wc.h"
#include "wordlist.h"

static int has_next(void * vx)
{
  return ! ! wordlist[++(* (int *) vx)];
}

static size_t len(void * vx)
{
  return strlen(wordlist[* (int*) vx]);
}

static void get(agram_dchar * out, void * vx)
{
  strcpy(out, wordlist[* (int *) vx]);
}

static const struct cwlcbs cbs = {has_next, len, get};

static void purge_wl(const char * const fn)
{
  char fname[strlen(fn) + 3];
#define RM(suffix) \
  sprintf(fname, "%s." # suffix, fn); \
  remove(fname);

  RM(i);
  RM(s);
  RM(c);
  RM(n);
  RM(k);
#undef RM

  remove(fn);
  fname[strlen(fn)-2] = 0;
  remove(fname);
}

static char * cachedir(void)
{
  const char * const cdir = getenv("XDG_CACHE_DIR");
  if (cdir && *cdir)
    return strdup(cdir);

  const char * const hdir = getenv("HOME");
  char * const hcdir = malloc(strlen(hdir) + strlen("/.cache") + 1);
  if (hcdir)
    sprintf(hcdir, "%s/.cache", hdir);
  return hcdir;
}

static int mkpdirs(const char * const pn)
{
  const size_t len = strlen(pn);
  char pdir[len + 1];
  strcpy(pdir, pn);
  for (size_t i = 1; i < sizeof(pdir); i++)
    if (pdir[i] == '/')
      {
        pdir[i] = 0;
        if (access(pdir, F_OK))
          if (mkdir(pdir, 0700))
            return 1;
        pdir[i] = '/';
      }
  return 0;
}

static char * wordlist_cachename(void)
{
  char * const cdir = cachedir();
  char * const cname = malloc(strlen(cdir) + strlen("/c768f4cc-dd34-4d7f-931d-122058c5f4ee/wordlist") + 1);
  if (cname)
    sprintf(cname, "%s/c768f4cc-dd34-4d7f-931d-122058c5f4ee/wordlist", cdir);
  free(cdir);
  return cname;
}

static char * wordlist_tmpname(void)
{
  char * tmpdir = getenv("TMPDIR");
  if (! tmpdir)
    tmpdir = "/tmp";
  char * const wlistfn = malloc(strlen(tmpdir) + strlen("/XXXXXX") + strlen(".k") + 1);
  if (wlistfn)
    {
      sprintf(wlistfn, "%s/XXXXXX", tmpdir);
      if (! mkdtemp(wlistfn))
        return NULL;
      strcat(wlistfn, "/x");
    }
  return wlistfn;
}

int init_wl(void)
{
  int purge = 0;
  char * wlistfn = wordlist_cachename();
  if (! wlistfn || mkpdirs(wlistfn))
    {
      wlistfn = wordlist_tmpname();
      purge = 1;
    }

  char ckfn[strlen(wlistfn) + strlen(".k") + 1];
  strcat(strcpy(ckfn, wlistfn), ".k");
  if (access(ckfn, F_OK))
    {
      int x = -1;
      if (compile_wl(wlistfn, &cbs, &x))
        return 1;
    }

  if (load_wl(wlistfn))
    return 1;

  if (purge)
    purge_wl(wlistfn);
  free(wlistfn);
  return 0;
}
