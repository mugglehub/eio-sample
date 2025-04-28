#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_show_therm
//=============================================================================
void eio_show_therm(void)
{
	int			i;
	uint8_t		res;
	uint8_t		data8;
	uint16_t	data16;
	uint8_t		sht_act;
	uint16_t	sht_high, sht_low;

	eio_fw_prt_t	prt;

	fprintf(stderr, "==================================\n");
	fprintf(stderr, "           EIO Thermal\n");
	fprintf(stderr, "==================================\n");

	for (i=0; i<EIO_MAX_THERM; i++)
	{
	
		prt.cmd = 0x11;
		prt.ctl = 0x00;
		prt.dev = (uint8_t)i;
		prt.len = 2;

		res = eio_fw_read_prt(&prt);

		if (res)
			continue;
		
		if ((prt.buf[0] & 0x01) == 0)
		{
			fprintf(stderr, "THERM%d > \n  n/a\n", i);
			continue;
		}

		fprintf(stderr, "THERM%d >\n", i);

		// type
		prt.cmd = 0x11;
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
		prt.cmd = 0x11;
		prt.ctl = 0x10;
		prt.dev = (uint8_t)i;
		prt.len = 2;

		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  Value: ");
		data16 = *(uint16_t *)prt.buf;
		if (res == EIO_OK)
			fprintf(stderr, "0x%04X = %d.%d K = %d.%d C\n", data16, data16/10, data16%10, (data16-2731)/10, (data16-2731)%10);
		else
			fprintf(stderr, "error, res=%d\n", res);

		//-----------------------------------------------------------
		//  TP : SHT
		//-----------------------------------------------------------
		// Act
		prt.cmd = 0x11;
		prt.ctl = 0x40;
		prt.dev = (uint8_t)i;
		prt.len = 1;

		res = eio_fw_read_prt(&prt);

		sht_act = 0xFF;
		if (!res)
			sht_act = prt.buf[0];

		// High
		prt.cmd = 0x11;
		prt.ctl = 0x41;
		prt.dev = (uint8_t)i;
		prt.len = 2;

		res = eio_fw_read_prt(&prt);

		sht_high = 0xFFFF;
		if (!res)
			sht_high = *(uint16_t *)prt.buf;

		// Low
		prt.cmd = 0x11;
		prt.ctl = 0x42;
		prt.dev = (uint8_t)i;
		prt.len = 2;

		res = eio_fw_read_prt(&prt);

		sht_low = 0xFFFF;
		if (!res)
			sht_low = *(uint16_t *)prt.buf;

		fprintf(stderr, "    Act: 0x%02X\n", sht_act);

		fprintf(stderr, "   High: 0x%04X = %d.%d C\n", 
				sht_high, (sht_high-2731)/10, (sht_high-2731)%10);
		fprintf(stderr, "    Low: 0x%04X = %d.%d C\n", 
				sht_low, (sht_low-2731)/10, (sht_low-2731)%10);
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

	eio_show_therm();

	eio_exit();

	return 0;
}
	
