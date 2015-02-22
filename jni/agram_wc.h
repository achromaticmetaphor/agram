#ifndef AGRAM_WC_H
#define AGRAM_WC_H

#include <stddef.h>

#include "lcwc.h"

#include <jni.h>

extern struct lc * words_counts;
extern size_t NWORDS;
extern jint * strbase;
extern jint * charsbase;
extern unsigned int * countsbase;

void unload_wl(void);
int load_wl(const char *);

#endif
