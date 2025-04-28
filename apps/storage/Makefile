CC=gcc
CFLAGS=-I.

all: eio_storage

eio_storage: eio_storage.c
	$(CC) $(CFLAGS) -o eio_storage eio_storage.c ../../eio/eio.c

clean:
	rm -f eio_storage
