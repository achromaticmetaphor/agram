.POSIX:
.SUFFIXES:
.SUFFIXES: .c .o

agram : agram.o agram_wc.o anagram.o anagrams.o is_anagram.o is_within.o lcwc.o lettercounts.o vector.o wc.o wordlist.o words_from.o xdgwl.o
	c99 -o agram agram.o agram_wc.o anagram.o anagrams.o is_anagram.o is_within.o lcwc.o lettercounts.o vector.o wc.o wordlist.o words_from.o xdgwl.o

.c.o :
	c99 -o $@ -c $<

wordlist.c : ../assets/words
	>wordlist.c echo 'const char * const wordlist[] ='
	>>wordlist.c echo '{'
	>>wordlist.c sed -e 's/^/"/' -e 's/$$/",/' ../assets/words
	>>wordlist.c echo '0'
	>>wordlist.c echo '};'
