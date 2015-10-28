#!/usr/bin/env python

import itertools
import re
import string
import sys
import tarfile
import unicodedata

def mangle_words(words):
  words = (''.join(c for c in unicodedata.normalize('NFKD', w) if c in string.printable) for w in words)
  words = (w for w in words if len(w) > 1 or w in ('a', 'i'))
  words = {w for w in words if all(c in string.ascii_lowercase for c in w)}
  return sorted(words, key=lambda w: (-len(w), w))

if __name__ == "__main__":
  if len(sys.argv) == 2:
    scowl = tarfile.open(sys.argv[1])
    wordlists = (scowl.extractfile(m) for m in scowl.getmembers() if m.isreg() and re.match(r'.*/final/english-words\.[123456]', m.name))
    words = itertools.chain.from_iterable(wordlists)
    words = (w.decode('ISO-8859-1').strip() for w in words)
    for w in mangle_words(words):
      print(w)
