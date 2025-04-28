CC=gcc
CFLAGS=-I.

all: eio_pwm

eio_pwm: eio_pwm.c
	$(CC) $(CFLAGS) -o eio_pwm eio_pwm.c ../../eio/eio.c

clean:
	rm -f eio_pwm
