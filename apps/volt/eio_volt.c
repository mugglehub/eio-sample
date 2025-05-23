#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_show_volt
//=============================================================================
void eio_show_volt(void)
{
	int			i;
	uint8_t		res;
	uint8_t		data8;
	uint16_t	data16;

	eio_fw_prt_t	prt;

	fprintf(stderr, "==================================\n");
	fprintf(stderr, "           EIO Voltage\n");
	fprintf(stderr, "==================================\n");

	for (i=0; i<EIO_MAX_VOLT; i++)
	{
	
		prt.cmd = 0x13;
		prt.ctl = 0x00;
		prt.dev = (uint8_t)i;
		prt.len = 1;

		res = eio_fw_read_prt(&prt);

		if (res)
			continue;

		if ((prt.buf[0] & 0x01) == 0)
		{
			fprintf(stderr, "VOLT%d > \n  n/a\n", i);
			continue;
		}

		fprintf(stderr, "VOLT%d >\n", i);

		// type
		prt.cmd = 0x13;
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
		
		// value
		prt.cmd = 0x13;
		prt.ctl = 0x10;
		prt.dev = (uint8_t)i;
		prt.len = 2;
		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  Value: ");
		data16 = *(uint16_t *)prt.buf;
		// unit: 10mV
		if (res == EIO_OK)
			fprintf(stderr, "0x%04X = %d mV = %d.%02d V\n", data16, data16*10, data16/100, data16%100);
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

	eio_show_volt();

	eio_exit();

	return 0;
}
	
