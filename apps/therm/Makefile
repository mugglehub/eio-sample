CC=gcc
CFLAGS=-I.

all: eio_therm

eio_therm: eio_therm.c
	$(CC) $(CFLAGS) -o eio_therm eio_therm.c ../../eio/eio.c

clean:
	rm -f eio_therm
