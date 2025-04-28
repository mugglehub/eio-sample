#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_show_pwm
//=============================================================================
void eio_show_pwm(void)
{
	int			i;
	uint8_t		res;
	uint8_t		data8;
	uint32_t	data32;

	eio_fw_prt_t	prt;

	fprintf(stderr, "==================================\n");
	fprintf(stderr, "           EIO PWM\n");
	fprintf(stderr, "==================================\n");

	for (i=0; i<EIO_MAX_PWM; i++)
	{
		// checking PWM available
		prt.cmd = 0x15;
		prt.ctl = 0x00;
		prt.dev = (uint8_t)i;
		prt.len = 1;

		res = eio_fw_read_prt(&prt);

		if (res)
			continue;

		if ((prt.buf[0] & 0x01) == 0)
		{
			fprintf(stderr, "PWM%d > \n  n/a\n", i);
			continue;
		}

		fprintf(stderr, "PWM%d >\n", i);

		// duty cycle
		prt.cmd = 0x15;
		prt.ctl = 0x10;
		prt.dev = (uint8_t)i;
		prt.len = 1;
		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  Duty:      ");
		data8 = *(uint8_t *)prt.buf;
		if (res == EIO_OK)
			fprintf(stderr, "0x%02X = %d %%\n", data8, data8);
		else
			fprintf(stderr, "error, res=%d\n", res);

		// polarity
		prt.cmd = 0x15;
		prt.ctl = 0x11;
		prt.dev = (uint8_t)i;
		prt.len = 1;
		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  Polarity:  ");
		data8 = *(uint8_t *)prt.buf;
		if (res == EIO_OK)
			fprintf(stderr, "%d\n", data8);
		else
			fprintf(stderr, "error, res=%d\n", res);

		// frequency
		prt.cmd = 0x15;
		prt.ctl = 0x12;
		prt.dev = (uint8_t)i;
		prt.len = 4;
		res = eio_fw_read_prt(&prt);

		fprintf(stderr, "  Frequency: ");
		data32 = *(uint32_t *)prt.buf;
		if (res == EIO_OK)
			fprintf(stderr, "0x%X = %d Hz\n", data32, data32);
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

	eio_show_pwm();

	eio_exit();

	return 0;
}
	
