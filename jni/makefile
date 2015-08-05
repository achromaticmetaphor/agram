.POSIX:
.SUFFIXES:
.SUFFIXES: .c .o

agram : agram.o agram_wc.o anagram.o anagrams.o is_anagram.o is_within.o lcwc.o lettercounts.o vector.o wc.o wordlist.o words_from.o xdgwl.o
	c99 -o agram agram.o agram_wc.o anagram.o anagrams.o is_anagram.o is_within.o lcwc.o lettercounts.o vector.o wc.o wordlist.o words_from.o xdgwl.o

.c.o :
	c99 -o $@ -c $<

agram.o : agram_wc.h anagram.h anagrams.h vector.h wc.h words_from.h xdgwl.h
agram_wc.o : agram_types.h
anagram.o : agram_types.h agram_wc.h is_anagram.h lcwc.h
anagrams.o : agram_types.h agram_wc.h is_anagram.h is_within.h lcwc.h
is_anagram.o : agram_wc.h lcwc.h
is_within.o : agram_wc.h lcwc.h
lcwc.o : agram_types.h agram_wc.h astr.h lcwc.h lettercounts.h
lettercounts.o : agram_types.h astr.h
vector.o : vector.h
wc.o : agram_types.h agram_wc.h lcwc.h lettercounts.h vector.h wc.h
words_from.o : agram_types.h agram_wc.h is_within.h lcwc.h
xdgwl.o : wc.h wordlist.h

wordlist.c : ../assets/words
	>wordlist.c echo 'const char * const wordlist[] ='
	>>wordlist.c echo '{'
	>>wordlist.c sed -e 's/^/"/' -e 's/$$/",/' ../assets/words
	>>wordlist.c echo '0'
	>>wordlist.c echo '};'
