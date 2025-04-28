#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_show_caseopen
//=============================================================================
void eio_show_caseopen(void)
{
	int			i;
	uint8_t		res;
	uint8_t		data8;

	eio_fw_prt_t	prt;

	fprintf(stderr, "==================================\n");
	fprintf(stderr, "           EIO Caseopen\n");
	fprintf(stderr, "==================================\n");

	for (i=0; i<EIO_MAX_CASEOPEN; i++)
	{
		// checking current available
		prt.cmd = 0x29;
		prt.ctl = 0x00;
		prt.dev = (uint8_t)i;
		prt.len = 1;

		res = eio_fw_read_prt(&prt);

		if (res)
			continue;

		if ((prt.buf[0] & 0x01) == 0)
		{
			fprintf(stderr, "CASEOPEN%d > \n  n/a\n", i);
			continue;
		}

		fprintf(stderr, "CASEOPEN%d >\n", i);

		// case open control/status
		prt.cmd = 0x29;
		prt.ctl = 0x02;
		prt.dev = (uint8_t)i;
		prt.len = 1;
		res = eio_fw_read_prt(&prt);

		data8 = *(uint8_t *)prt.buf;
		if (res == EIO_OK)
		{
			if (data8 & 0x80)
			{
				fprintf(stderr, "Status : n/a\n");
			}
			else
			{
				fprintf(stderr, "Status : 0x%02X\n", data8);
				
				fprintf(stderr, "Switch : ");
				if (data8 & 0x01)
					fprintf(stderr, "Opened\n");
				else
					fprintf(stderr, "Closed\n");
			
				fprintf(stderr, "Power  : ");
				if (data8 & 0x02)
					fprintf(stderr, "Failed\n");
				else
					fprintf(stderr, "Good\n");
			}
		}
		else
		{
			fprintf(stderr, "error, res=%d\n", res);
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

	eio_show_caseopen();

	eio_exit();

	return 0;
}
	
