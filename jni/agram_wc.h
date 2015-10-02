#ifndef AGRAM_WC_H
#define AGRAM_WC_H

#include "agram_types.h"
#include "lcwc.h"

extern struct lc * words_counts;
extern size_t words_counts_len;
extern agram_size NWORDS;
extern agram_dchar * strbase;
extern size_t strbase_len;
extern agram_cpt * charsbase;
extern size_t charsbase_len;
extern unsigned int * countsbase;
extern size_t countsbase_len;

void unload_wl(void);
int load_wl(const char *);

#endif
