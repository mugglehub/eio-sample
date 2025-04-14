CC=gcc
CFLAGS=-I.

all: eio_ecram

eio_ecram: eio_ecram.c
	$(CC) $(CFLAGS) -o eio_ecram eio_ecram.c ../../eio/eio.c

clean:
	rm -f eio_ecram
