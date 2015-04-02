#ifndef AGRAM_WC_H
#define AGRAM_WC_H

#include "agram_types.h"
#include "lcwc.h"

extern struct lc * words_counts;
extern agram_size NWORDS;
extern agram_dchar * strbase;
extern agram_cpt * charsbase;
extern unsigned int * countsbase;

void unload_wl(void);
int load_wl(const char *);

#endif
