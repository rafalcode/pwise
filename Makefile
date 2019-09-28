CC=gcc
CFLAGS=-g -Wall
DBGCFLAGS=-g -Wall -DDBG
SPECLIBS=
BZLIBS=-lbz2

EXECUTABLES=pwise0

pwise0: pwise0.c
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean

clean:
	rm -f ${EXECUTABLES}
