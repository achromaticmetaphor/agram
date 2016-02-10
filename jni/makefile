.POSIX:
.SUFFIXES:
.SUFFIXES: .c .o

agram : agram.o agram_wc.o anagram.o anagrams.o is_anagram.o is_within.o lcwc.o lettercounts.o vector.o wc.o words_from.o xdgwl.o
	c99 -o agram agram.o agram_wc.o anagram.o anagrams.o is_anagram.o is_within.o lcwc.o lettercounts.o vector.o wc.o words_from.o xdgwl.o

.c.o :
	c99 -o $@ -c $<

agram.o : agram_types.h anagram.h anagrams.h lcwc.h vector.h wc.h wordlist.h words_from.h xdgwl.h
agram_wc.o : agram_types.h lcwc.h wordlist.h
anagram.o : agram_types.h is_anagram.h lcwc.h wordlist.h
anagrams.o : agram_types.h anagrams.h is_within.h lcwc.h wordlist.h
astr.o : agram_types.h
is_anagram.o : agram_types.h lcwc.h wordlist.h
is_within.o : agram_types.h lcwc.h wordlist.h
lcwc.o : agram_types.h astr.h lcwc.h lettercounts.h wordlist.h
lettercounts.o : agram_types.h astr.h
vector.o : vector.h
wc.o : agram_types.h agram_wc.h lcwc.h lettercounts.h vector.h wc.h wordlist.h
words_from.o : agram_types.h is_within.h lcwc.h wordlist.h
xdgwl.o : agram_types.h lcwc.h wc.h wordlist.h
