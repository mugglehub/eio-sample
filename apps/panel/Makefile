CC=gcc
CFLAGS=-I.

all: eio_panel

eio_panel: eio_panel.c
	$(CC) $(CFLAGS) -o eio_panel eio_panel.c ../../eio/eio.c

clean:
	rm -f eio_panel
