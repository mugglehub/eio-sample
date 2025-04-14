CC=gcc
CFLAGS=-I.

all: eio_volt

eio_volt: eio_volt.c
	$(CC) $(CFLAGS) -o eio_volt eio_volt.c ../../eio/eio.c

clean:
	rm -f eio_volt
