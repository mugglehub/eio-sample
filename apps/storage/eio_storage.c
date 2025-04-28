#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_show_storage
//=============================================================================
uint8_t eio_show_storage(void)
{
	int			i, j;
	uint8_t		res;
	uint8_t		storage[128];

	eio_fw_prt_t	prt;

	fprintf(stderr, "=========================================================================\n");
	fprintf(stderr, "                        EIO Storage\n");
	fprintf(stderr, "=========================================================================\n");

	// checking current available
	prt.cmd = 0x51;
	prt.ctl = 0x00;
	prt.dev = (uint8_t)i;
	prt.len = 1;

	res = eio_fw_read_prt(&prt);

	if (res)
		return res;

	if ((prt.buf[0] & 0x01) == 0)
	{
		fprintf(stderr, "STORAGE > \n  n/a\n");
		return 0xFF;
	}

	fprintf(stderr, "STORAGE >\n");

	// storage 00h~3Fh
	prt.cmd = 0x51;
	prt.ctl = 0x05;
	prt.dev = 0x00;	// from 00h~3Fh
	prt.len = 64;	// max=64
	res = eio_fw_read_prt(&prt);

	if (res == EIO_OK)
		memcpy(&storage[0], prt.buf, 64);
	else
		fprintf(stderr, "error, res=%d\n", res);

	// storage 00h~3Fh
	prt.cmd = 0x51;
	prt.ctl = 0x05;
	prt.dev = 0x40;	// from 00h~3Fh
	prt.len = 64;	// max=64
	res = eio_fw_read_prt(&prt);

	if (res == EIO_OK)
		memcpy(&storage[0x40], prt.buf, 64);
	else
		fprintf(stderr, "error, res=%d\n", res);

	// showing
	for (i=0; i<128; i++)
	{
		if (i%16==0)
			fprintf(stderr, "%04X  ", i);
		fprintf(stderr, "%02X ", storage[i]);
		if (i%16==7)
		{
			fprintf(stderr, "- ");
		}
		else if (i%16==15)
		{
			fprintf(stderr, " ");
			for (j=i-15; j<=i; j++)
			{
				if ((storage[j] < 0x20) || (storage[j] > 0x7e))
					fprintf(stderr, ".");
				else
					fprintf(stderr, "%c", storage[j]);
			}
			fprintf(stderr, "\n");
		}
	}

	fprintf(stderr, "\n");
	
	fprintf(stderr, "Example : write data 07h in address[0]\n");
	fprintf(stderr, "\teio_storage 0 7\n");
	
	return 0;
}

//=============================================================================
//  eio_write_storage
//=============================================================================
uint8_t eio_write_storage(char *addr, char *data)
{
	uint8_t		res;
	uint8_t 	a = atoi(addr);	// in decimal
	uint8_t 	d = atoi(data);	// in decimal

	eio_fw_prt_t	prt;

	if (a < 0x80)
	{
		// storage 00h~3Fh
		prt.cmd = 0x50;
		prt.ctl = 0x05;
		prt.dev = a;	// 00h~7Fh
		prt.len = 1;
		prt.buf[0] = d;
		res = eio_fw_write_prt(&prt);
	
		if (res == EIO_OK)
			fprintf(stderr, "Write storage [%02Xh] = %02X ok!\n", a, d);
		else
			fprintf(stderr, "error, res=%d\n", res);
	}
}

//=============================================================================
//  main
//=============================================================================
uint8_t main(int argc, char *argv[])
{
	uint8_t res;
	
	res = eio_init();

	if (res)
		return res;

	if (argc == 3)
	{
		// <addr> <data>
		eio_write_storage(argv[1], argv[2]);
	}
	else
	{
		eio_show_storage();
	}

	eio_exit();

	return 0;
}
	
