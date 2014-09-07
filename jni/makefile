.POSIX:
.SUFFIXES:
.SUFFIXES: .c .o

CC = gcc -Wall -Wextra -Werror --std=iso9899:1999 -pedantic -pipe -O2 -ggdb -Wl,--build-id=sha1

agram_wc.c : wc
	./wc >agram_wc.c

wc : wc.o lettercounts.o
	$(CC) -o wc wc.o lettercounts.o

wc.o : lettercounts.h mlen.h words.h

.c.o :
	$(CC) -o $@ -c $<

words.h : words
	>words.h echo '#include <stddef.h>'
	>>words.h echo
	>>words.h echo 'static const char * const words[] ='
	>>words.h echo '{'
	>>words.h sed -e 's/^/"/' -e 's/$$/",/' words
	>>words.h echo '};'
	>>words.h echo
	>>words.h echo 'static const size_t NWORDS = sizeof(words)/sizeof(words[0]);'
