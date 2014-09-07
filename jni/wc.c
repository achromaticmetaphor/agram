#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <search.h>

#include "lettercounts.h"
#include "words.h"
#include "mlen.h"

MLEN_DECL(uint, unsigned int)

struct ll
{
  unsigned int * counts;
  size_t ncounts;
  size_t orig_offset;
};

static int ll_wc_counts_probe(const void * const a, const void * const b)
{
  const struct ll * const node = a;
  const unsigned int * const counts = b;
  return memcmp(node->counts, counts, node->ncounts * sizeof(*counts));
}

static size_t total_counts(const struct ll nodes[], const size_t len)
{
  size_t sum = 0;
  for (size_t i = 0; i < len; i++)
    sum += nodes[i].ncounts;
  return sum;
}

static int counts_cmp(const struct ll * const a, const struct ll * const b)
{
  return a->ncounts < b->ncounts
           ? -1
           : a->ncounts != b->ncounts;
}

static int ll_counts_cmp(const void * const a, const void * const b)
{
  return counts_cmp(b, a);
}

static void ll_make(unsigned int * const * const counts, struct ll * const nodes, const size_t nwords)
{
  for (size_t i = 0; i < nwords; i++)
    {
      nodes[i].orig_offset = i;
      nodes[i].counts = counts[i];
      nodes[i].ncounts = mlen_uint(nodes[i].counts);
    }
}

int main(void)
{
  puts("#include <stddef.h>");
  puts("#include <limits.h>");
  putchar('\n');
  puts("#include \"lcwc.h\"");
  putchar('\n');

  char * * const chars = malloc(NWORDS * sizeof(*chars));
  unsigned int * * const counts = malloc(sizeof(*counts) * NWORDS);

  for (size_t i = 0; i < NWORDS; i++)
    {
      const size_t len = strlen(words[i]) + 1;
      counts[i] = malloc(sizeof(*counts[i]) * len);
      chars[i] = malloc(sizeof(*chars[i]) * len);
      lettercounts(counts[i], chars[i], words[i]);
    }

  struct ll * const nodes = malloc(sizeof(*nodes) * NWORDS);
  ll_make(counts, nodes, NWORDS);
  free(counts);

  unsigned int * wc_counts = malloc(sizeof(*wc_counts) * total_counts(nodes, NWORDS));
  size_t counts_offset = 0;

  qsort(nodes, NWORDS, sizeof(*nodes), ll_counts_cmp);
  size_t * const counts_offsets = malloc(sizeof(*counts_offsets) * NWORDS);
  for (size_t i = 0; i < NWORDS; i++)
    {
      size_t search_len = counts_offset - nodes[i].ncounts;
      unsigned int * found = counts_offset > nodes[i].ncounts
                               ? lfind(nodes+i, wc_counts, &search_len, sizeof(*wc_counts), ll_wc_counts_probe)
                               : NULL;
      if (found)
        {
          const size_t offset = found - wc_counts;
          counts_offsets[nodes[i].orig_offset] = offset;
        }
      else
        {
          counts_offsets[nodes[i].orig_offset] = counts_offset;
          memcpy(wc_counts + counts_offset, nodes[i].counts, nodes[i].ncounts * sizeof(*wc_counts));
          counts_offset += nodes[i].ncounts;
        }
      free(nodes[i].counts);
    }

  free(nodes);

  puts("static unsigned int wc_counts[] = {");
  for (size_t i = 0; i < counts_offset; i++)
    printf("%d,\n", wc_counts[i]);
  puts("};");
  putchar('\n');

  free(wc_counts);

  puts("const struct wc words_counts[] = {");
  for (size_t i = 0; i < NWORDS; i++)
    {
      printf("{%u, ", (unsigned int) strlen(words[i]));
      printf("%u,\n", (unsigned int) strlen(chars[i]));
      printf("\"%s\",\n", words[i]);
      printf("\"%s\",\n", chars[i]);
      printf("wc_counts+%u},\n", (unsigned int) counts_offsets[i]);
      free(chars[i]);
    }
  puts("};");
  putchar('\n');

  free(chars);
  free(counts_offsets);

  puts("const size_t NWORDS = sizeof(words_counts)/sizeof(words_counts[0]);");
  putchar('\n');

  return 0;
}
