.POSIX:
.SUFFIXES:
.SUFFIXES: .c .o

agram : agram.o ../csrc/agram_wc.o ../csrc/anagram.o ../csrc/anagrams.o ../csrc/is_anagram.o ../csrc/is_within.o ../csrc/lcwc.o ../csrc/lettercounts.o ../csrc/vector.o ../csrc/wc.o ../csrc/words_from.o xdgwl.o
	c99 -o agram agram.o ../csrc/agram_wc.o ../csrc/anagram.o ../csrc/anagrams.o ../csrc/is_anagram.o ../csrc/is_within.o ../csrc/lcwc.o ../csrc/lettercounts.o ../csrc/vector.o ../csrc/wc.o ../csrc/words_from.o xdgwl.o

.c.o :
	c99 -I../csrc -o $@ -c $<

agram.o: ../csrc/anagram.h ../csrc/agram_types.h ../csrc/wordlist.h ../csrc/anagrams.h ../csrc/lcwc.h ../csrc/vector.h ../csrc/wc.h ../csrc/wordlist.h ../csrc/words_from.h xdgwl.h
xdgwl.o: ../csrc/wc.h ../csrc/agram_types.h ../csrc/lcwc.h ../csrc/wordlist.h ../csrc/wordlist.h
../csrc/agram_wc.o: ../csrc/agram_types.h ../csrc/lcwc.h ../csrc/wordlist.h
../csrc/anagram.o: ../csrc/agram_types.h ../csrc/is_anagram.h ../csrc/lcwc.h ../csrc/wordlist.h
../csrc/anagrams.o: ../csrc/agram_types.h ../csrc/anagrams.h ../csrc/lcwc.h ../csrc/wordlist.h ../csrc/is_within.h
../csrc/is_anagram.o: ../csrc/lcwc.h ../csrc/agram_types.h ../csrc/wordlist.h
../csrc/is_within.o: ../csrc/lcwc.h ../csrc/agram_types.h ../csrc/wordlist.h
../csrc/lcwc.o: ../csrc/agram_types.h ../csrc/lcwc.h ../csrc/wordlist.h ../csrc/lettercounts.h
../csrc/lettercounts.o: ../csrc/agram_types.h
../csrc/vector.o: ../csrc/vector.h
../csrc/wc.o: ../csrc/agram_types.h ../csrc/agram_wc.h ../csrc/wordlist.h ../csrc/lcwc.h ../csrc/lettercounts.h ../csrc/vector.h ../csrc/wc.h
../csrc/words_from.o: ../csrc/agram_types.h ../csrc/is_within.h ../csrc/lcwc.h ../csrc/wordlist.h
