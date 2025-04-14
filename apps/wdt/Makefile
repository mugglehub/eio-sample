CC=gcc
CFLAGS=-I.

all: eio_wdt

eio_wdt: eio_wdt.c
	$(CC) $(CFLAGS) -o eio_wdt eio_wdt.c ../../eio/eio.c

clean:
	rm -f eio_wdt
