#ifndef LETTERCOUNTS_H
#define LETTERCOUNTS_H

#include <cstddef>

#include "agram_types.h"

void lettercounts(std::vector<unsigned int> &, std::vector<agram_codepoint> &,
                  const agram_display_char *, size_t);

#endif
