CC=gcc
CFLAGS=-I.

all: eio_caseopen

eio_caseopen: eio_caseopen.c
	$(CC) $(CFLAGS) -o eio_caseopen eio_caseopen.c ../../eio/eio.c

clean:
	rm -f eio_caseopen
