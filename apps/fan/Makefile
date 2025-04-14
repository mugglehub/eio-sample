CC=gcc
CFLAGS=-I.

all: eio_fan

eio_fan: eio_fan.c
	$(CC) $(CFLAGS) -o eio_fan eio_fan.c ../../eio/eio.c

clean:
	rm -f eio_fan
