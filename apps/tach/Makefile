CC=gcc
CFLAGS=-I.

all: eio_tach

eio_tach: eio_tach.c
	$(CC) $(CFLAGS) -o eio_tach eio_tach.c ../../eio/eio.c

clean:
	rm -f eio_tach
