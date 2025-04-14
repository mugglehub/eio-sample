#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_hw_gpio
//=============================================================================
void eio_hw_gpio(void)
{
	uint16_t	base;
	uint16_t	gpio_dat;
	uint16_t	gpio_dir;
	int			i;


	fprintf(stderr, "GPIO Cfg : irq=%d, base=0x%04X\n", eio_gpio_irq, eio_gpio_base);

	gpio_dat = inw(eio_gpio_base);
	gpio_dir = inw(eio_gpio_base + 0x04);
	fprintf(stderr, "GPIO Dat : 0x%04X\n", gpio_dat);
	fprintf(stderr, "GPIO Dir : 0x%04X\n\n", gpio_dir);
	
	fprintf(stderr, "H:High, L:Low, I:Input, O:Output\n\n");

	fprintf(stderr, "    Pin# : F E D C B A 9 8   7 6 5 4 3 2 1 0\n");
	fprintf(stderr, "GPIO Dat : ");
	for (i=15; i>=0; i--)
	{
		if (gpio_dat & (1<<i))
			fprintf(stderr, "1 ");
		else
			fprintf(stderr, "0 ");
		if (i==8)
			fprintf(stderr, "- ");
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "           ");
	for (i=15; i>=0; i--)
	{
		if (gpio_dat & (1<<i))
			fprintf(stderr, "H ");
		else
			fprintf(stderr, "L ");
		if (i==8)
			fprintf(stderr, "- ");
	}
	fprintf(stderr, "\n");


	fprintf(stderr, "GPIO Dir : ");
	for (i=15; i>=0; i--)
	{
		if (gpio_dir & (1<<i))
			fprintf(stderr, "1 ");
		else
			fprintf(stderr, "0 ");
		if (i==8)
			fprintf(stderr, "- ");
	}

	fprintf(stderr, "\n");

	fprintf(stderr, "           ");
	for (i=15; i>=0; i--)
	{
		if (gpio_dir & (1<<i))
			fprintf(stderr, "O ");
		else
			fprintf(stderr, "I ");
		if (i==8)
			fprintf(stderr, "- ");
	}

	fprintf(stderr, "\n\n");
	
	fprintf(stderr, "Usage:\n\teio_gpio -h      : for help\n");
}

//=============================================================================
//  main
//=============================================================================
uint8_t main(int argc, char *argv[])
{
	uint8_t 	res;
	uint8_t		pin;
	uint8_t		lvl;
	uint8_t		dir;
	uint16_t	val;

	res = eio_init();
	if (res)
		return res;
	
	res = eio_gpio_init();
	if (res)
		return res;

	if (argc == 2 && strcmp(argv[1], "-h") == 0)
	{
		fprintf(stderr, "Usage:\n\n");
		fprintf(stderr, "\teio_gpio <pin> <in|out|high|low>\n");
		fprintf(stderr, "\t  <pin> : 0 ~ 15\n\n");
	}
	else if (argc == 3)
	{
		// eio_gpio <pin> <dir>
		pin = atoi(argv[1]);

		if (pin < 16)
		{
			if (strcmp(argv[2], "in") == 0)
			{
				val = inw(eio_gpio_base + 0x04);
				val &= ~(1<<pin);
				outw(val, eio_gpio_base + 0x04);
				
				fprintf(stderr, "set gpio %d dir : in\n", pin);
			}
			else if (strcmp(argv[2], "out") == 0)
			{
				val = inw(eio_gpio_base + 0x04);
				val |= (1<<pin);
				outw(val, eio_gpio_base + 0x04);
				
				fprintf(stderr, "set gpio %d dir : out\n", pin);
			}
			else if (strcmp(argv[2], "low") == 0)
			{
				val = inw(eio_gpio_base);
				val &= ~(1<<pin);
				outw(val, eio_gpio_base);
				
				fprintf(stderr, "set gpio %d dat : low\n", pin);
			}
			else if (strcmp(argv[2], "high") == 0)
			{
				val = inw(eio_gpio_base);
				val |= (1<<pin);
				outw(val, eio_gpio_base);
				
				fprintf(stderr, "set gpio %d dat : high\n", pin);
			}
		}
	}
	else
	{
		eio_hw_gpio();
	}

	eio_gpio_exit();
	eio_exit();

	return 0;
}
	
