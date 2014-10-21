.POSIX:
.SUFFIXES:
.SUFFIXES: .c .o

agram_wc.c : wc
	./wc >agram_wc.c

wc : wc.o lettercounts.o
	c89 -o wc wc.o lettercounts.o

wc.o : lettercounts.h mlen.h words.h

.c.o :
	c89 -o $@ -c $<

words.h : words
	>words.h echo '#include <stddef.h>'
	>>words.h echo
	>>words.h echo 'static const char * const words[] ='
	>>words.h echo '{'
	>>words.h sed -e 's/^/"/' -e 's/$$/",/' words
	>>words.h echo '};'
	>>words.h echo
	>>words.h echo 'static const size_t NWORDS = sizeof(words)/sizeof(words[0]);'
