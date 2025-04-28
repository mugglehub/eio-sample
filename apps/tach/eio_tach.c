#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_show_tach
//=============================================================================
void eio_show_tach(void)
{
	int			i;
	uint8_t		res;
	uint8_t		data8;
	uint32_t	data32;

	eio_fw_prt_t	prt;

	fprintf(stderr, "==================================\n");
	fprintf(stderr, "           EIO TACH\n");
	fprintf(stderr, "==================================\n");

	for (i=0; i<EIO_MAX_TACH; i++)
	{
		// checking TACH available
		prt.cmd = 0x17;
		prt.ctl = 0x00;
		prt.dev = (uint8_t)i;
		prt.len = 1;

		res = eio_fw_read_prt(&prt);

		if (res)
			continue;

		if ((prt.buf[0] & 0x01) == 0)
		{
			fprintf(stderr, "TACH%d > \n  n/a\n", i);
			continue;
		}

		fprintf(stderr, "TACH%d >\n", i);

		// type
		prt.cmd = 0x17;
		prt.ctl = 0x01;
		prt.dev = (uint8_t)i;
		prt.len = 1;
		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  Type:  ");
		data8 = *(uint8_t *)prt.buf;
		if (res == EIO_OK)
			fprintf(stderr, "0x%02X\n", data8);
		else
			fprintf(stderr, "error, res=%d\n", res);

		// pulse
		prt.cmd = 0x17;
		prt.ctl = 0x03;
		prt.dev = (uint8_t)i;
		prt.len = 1;
		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  Pulse: ");
		data8 = *(uint8_t *)prt.buf;
		if (res == EIO_OK)
			fprintf(stderr, "0x%02X\n", data8);
		else
			fprintf(stderr, "error, res=%d\n", res);

		// rpm
		prt.cmd = 0x17;
		prt.ctl = 0x10;
		prt.dev = (uint8_t)i;
		prt.len = 4;
		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  RPM:   ");
		data32 = *(uint32_t *)prt.buf;
		if (res == EIO_OK)
			fprintf(stderr, "0x%X = %d RPM\n", data32, data32);
		else
			fprintf(stderr, "error, res=%d\n", res);
	}

}

//=============================================================================
//  main
//=============================================================================
uint8_t main(void)
{
	uint8_t res;
	
	res = eio_init();

	if (res)
		return res;

	eio_show_tach();

	eio_exit();

	return 0;
}
	
