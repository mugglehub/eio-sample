#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_show_ecram
//=============================================================================
void eio_show_ecram(void)
{
	int			i, j;
	uint8_t		res;
	uint8_t		ram[256];

	fprintf(stderr, "=========================================================================\n");
	fprintf(stderr, "                        ACPI EC RAM\n");
	fprintf(stderr, "=========================================================================\n");

	for (i=0; i<256; i++)
	{
		fprintf(stderr, "reading EC RAM : %d/256\r", i);
		res = acpi_ec_read((uint8_t)i, &ram[i]);
		if (res != ACPI_OK)
			fprintf(stderr, "\nEC_RAM[%02Xh] res = %d, (1=OBF err, 2=IBF err)\n", i, res);
	}


	for (i=0; i<256; i++)
	{
		if (i%16==0)
			fprintf(stderr, "%04X  ", i);
		fprintf(stderr, "%02X ", ram[i]);
		if (i%16==7)
		{
			fprintf(stderr, "- ");
		}
		else if (i%16==15)
		{
			fprintf(stderr, " ");
			for (j=i-15; j<=i; j++)
			{
				if ((ram[j] < 0x20) || (ram[j] > 0x7e))
					fprintf(stderr, ".");
				else
					fprintf(stderr, "%c", ram[j]);
			}
			fprintf(stderr, "\n");
		}
	}

	fprintf(stderr, "\n");
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

	eio_show_ecram();

	eio_exit();

	return 0;
}
	
