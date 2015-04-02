#ifndef WC_H
#define WC_H

#include <stddef.h>

#include <jni.h>

struct cwlcbs
{
  int (* has_next)(void *);
  size_t (* len)(void *);
  void (* get)(jchar *, void *);
};

jboolean compile_wl(const char *, struct cwlcbs const *, void *);

#endif
