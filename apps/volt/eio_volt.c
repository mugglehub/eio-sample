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
	uint16_t	data16;

	eio_fw_prt_t	prt;

	fprintf(stderr, "==================================\n");
	fprintf(stderr, "           EIO Voltage\n");
	fprintf(stderr, "==================================\n");

	for (i=0; i<8; i++)
	{
	
		prt.cmd = 0x13;
		prt.ctl = 0x00;
		prt.dev = (uint8_t)i;
		prt.len = 1;

		res = eio_fw_read_prt(&prt);

		if (res)
			continue;

		if ((prt.buf[0] & 0x01) == 0)
			continue;	// n/a

		fprintf(stderr, "VOLT: %d\n", i);

		prt.cmd = 0x13;
		prt.ctl = 0x10;
		prt.dev = (uint8_t)i;
		prt.len = 2;

		res = eio_fw_read_prt(&prt);

		if (!res)
		{
			// unit: 10mV
			data16 = *(uint16_t *)prt.buf;
			fprintf(stderr, "  Value: 0x%04X = %d.%02d V\n", 
					data16, data16/100, data16%100);
		}
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
	
