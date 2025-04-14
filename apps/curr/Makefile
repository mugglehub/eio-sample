CC=gcc
CFLAGS=-I.

all: eio_curr

eio_curr: eio_curr.c
	$(CC) $(CFLAGS) -o eio_curr eio_curr.c ../../eio/eio.c

clean:
	rm -f eio_curr
