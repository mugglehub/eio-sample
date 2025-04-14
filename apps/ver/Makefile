CC=gcc
CFLAGS=-I.

all: eio_ver

eio_ver: eio_ver.c
	$(CC) $(CFLAGS) -o eio_ver eio_ver.c ../../eio/eio.c

clean:
	rm -f eio_ver
