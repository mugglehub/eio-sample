CC=gcc
CFLAGS=-I.

all: eio_gpio

eio_gpio: eio_gpio.c
	$(CC) $(CFLAGS) -o eio_gpio eio_gpio.c ../../eio/eio.c

clean:
	rm -f eio_gpio
