#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_show_panel
//=============================================================================
void eio_show_panel(void)
{
	int			i;
	uint8_t		res;
	uint8_t		data8;

	eio_fw_prt_t	prt;

	fprintf(stderr, "==================================\n");
	fprintf(stderr, "           EIO Panel\n");
	fprintf(stderr, "==================================\n");

	for (i=0; i<EIO_MAX_PANEL; i++)
	{
		// checking panel available
		prt.cmd = 0x21;
		prt.ctl = 0x00;
		prt.dev = (uint8_t)i;
		prt.len = 1;

		res = eio_fw_read_prt(&prt);

		if (res)
			continue;

		if ((prt.buf[0] & 0x01) == 0)
		{
			fprintf(stderr, "PANEL%d > \n  n/a\n", i);
			continue;
		}

		fprintf(stderr, "PANEL%d >\n", i);

		// duty cycle
		prt.cmd = 0x21;
		prt.ctl = 0x14;
		prt.dev = (uint8_t)i;
		prt.len = 1;
		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  Duty:     ");
		data8 = *(uint8_t *)prt.buf;
		if (res == EIO_OK)
			fprintf(stderr, "0x%02X = %d %%\n", data8, data8);
		else
			fprintf(stderr, "error, res=%d\n", res);
		
		// polarity
		prt.cmd = 0x21;
		prt.ctl = 0x15;
		prt.dev = (uint8_t)i;
		prt.len = 1;
		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  Polarity: ");
		data8 = *(uint8_t *)prt.buf;
		if (res == EIO_OK)
			fprintf(stderr, "%d\n", data8);
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

	eio_show_panel();

	eio_exit();

	return 0;
}
	
