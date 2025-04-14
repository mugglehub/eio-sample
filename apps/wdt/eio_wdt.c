#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

#define EIO_WDT_DELAY	0x11
#define EIO_WDT_PWRCYC	0x12
#define EIO_WDT_IRQ		0x13
#define EIO_WDT_RESET	0x14
#define EIO_WDT_PIN		0x15
#define EIO_WDT_SCI		0x16

//=============================================================================
//  eio_wdt_start
//=============================================================================
void eio_wdt_start(void)
{
	uint8_t			res;
	eio_fw_prt_t	prt;

	prt.cmd = 0x2A;
	prt.ctl = 0x02;
	prt.dev = 0;
	prt.len = 1;
	prt.buf[0] = 0x01;

	res = eio_fw_write_prt(&prt);

	if (!res)
		fprintf(stderr, "EIO wdt start ok!\n");
	else
		fprintf(stderr, "EIO wdt start err!\n");
}

//=============================================================================
//  eio_wdt_stop
//=============================================================================
void eio_wdt_stop(void)
{
	uint8_t			res;
	eio_fw_prt_t	prt;

	prt.cmd = 0x2A;
	prt.ctl = 0x02;
	prt.dev = 0;
	prt.len = 1;
	prt.buf[0] = 0x00;

	res = eio_fw_write_prt(&prt);

	if (!res)
		fprintf(stderr, "EIO wdt stop ok!\n");
	else
		fprintf(stderr, "EIO wdt stop err!\n");
}

//=============================================================================
//  eio_wdt_clear
//=============================================================================
void eio_wdt_clear(void)
{
	uint8_t			res;
	eio_fw_prt_t	prt;

	prt.cmd = 0x2A;
	prt.ctl = 0x02;
	prt.dev = 0;
	prt.len = 1;
	prt.buf[0] = 0x02;

	res = eio_fw_write_prt(&prt);

	if (!res)
		fprintf(stderr, "EIO wdt clear ok!\n");
	else
		fprintf(stderr, "EIO wdt clear err!\n");
}


//=============================================================================
//  eio_wdt_set
//=============================================================================
void eio_wdt_set(uint8_t wdt_type, uint32_t wdt_tmo)
{
	uint8_t			res;
	eio_fw_prt_t	prt;

	if (wdt_type < EIO_WDT_DELAY || wdt_type > EIO_WDT_SCI)
	{
		fprintf(stderr, "Watchdog Type error : 0x%02X\n", wdt_type);
		return;
	}

	prt.cmd = 0x2A;
	prt.ctl = wdt_type;
	prt.dev = 0;
	prt.len = 4;
	prt.buf[0] = (wdt_tmo >> 0) & 0xFF;
	prt.buf[1] = (wdt_tmo >> 8) & 0xFF;
	prt.buf[2] = (wdt_tmo >> 16) & 0xFF;
	prt.buf[3] = (wdt_tmo >> 24) & 0xFF;

	res = eio_fw_write_prt(&prt);

	if (!res)
		fprintf(stderr, "EIO wdt set ok!\n");
	else
		fprintf(stderr, "EIO wdt set err!\n");
}

//=============================================================================
//  eio_show_wdt
//=============================================================================
void eio_show_wdt(void)
{
	uint8_t			res;
	eio_fw_prt_t	prt;
	uint32_t		*tmo;
	uint8_t			wdt;
	char			wdt_name[6][8] =
	{"DELAY", "  PWR", "  IRQ", "  RST", "  PIN", "  SCI" };

	fprintf(stderr, "==================================\n");
	fprintf(stderr, "           EIO Watchdog\n");
	fprintf(stderr, "==================================\n");
	
	// 00	
	prt.cmd = 0x2B;	prt.ctl = 0x00;	prt.dev = 0;	prt.len = 1;
	res = eio_fw_read_prt(&prt);
	printf("WDT STS : %02Xh\n", prt.buf[0]);

	// 02
	prt.cmd = 0x2B;	prt.ctl = 0x02;	prt.dev = 0;	prt.len = 1;
	res = eio_fw_read_prt(&prt);
	printf("WDT CTL : %02Xh\n", prt.buf[0]);

	// 10
	prt.cmd = 0x2B;	prt.ctl = 0x10;	prt.dev = 0;	prt.len = 1;
	res = eio_fw_read_prt(&prt);
	printf("WDT EVT : %02Xh\n", prt.buf[0]);

	for (wdt=0x11; wdt<0x17; wdt++)
	{
		prt.cmd = 0x2B;
		prt.ctl = wdt;
		prt.dev = 0;
		prt.len = 4;

		res = eio_fw_read_prt(&prt);
		tmo = (uint32_t *)prt.buf;
		printf("WDT %s %02X : %d ms\n", wdt_name[wdt-0x11], wdt, *tmo);
	}

}

//=============================================================================
//  main
//=============================================================================
uint8_t main(int argc, char *argv[])
{
	uint8_t 	res;
	uint8_t		wdt_type;
	uint32_t	wdt_tmo;
	
	res = eio_init();

	if (res)
		return res;

	if (argc == 2)
	{
		if (strcmp(argv[1], "start") == 0)
		{
			eio_wdt_start();
		}
		else if (strcmp(argv[1], "stop") == 0)
		{
			eio_wdt_stop();
		}
		else if (strcmp(argv[1], "clear") == 0)
		{
			eio_wdt_clear();
		}
		else if (strcmp(argv[1], "help") == 0)
		{
			// usage
			fprintf(stderr, "Usage :\n\n");
			fprintf(stderr, "eio_wdt          : show wdt status\n");
			fprintf(stderr, "eio_wdt start    : watchdog start\n");
			fprintf(stderr, "eio_wdt stop     : watchdog stop\n");
			fprintf(stderr, "eio_wdt clear    : watchdog clear\n");
			fprintf(stderr, "eio_wdt help     : watchdog help\n\n");

			fprintf(stderr, "eio_wdt dly <ms> : set delay timeout in milli-seconds\n");
			fprintf(stderr, "eio_wdt pwr <ms> : set power cycle timeout in milli-seconds\n");
			fprintf(stderr, "eio_wdt rst <ms> : set reset timeout in milli-seconds\n");
			fprintf(stderr, "eio_wdt irq <ms> : set IRQ timeout in milli-seconds\n");
			fprintf(stderr, "eio_wdt sci <ms> : set SCI timeout in milli-seconds\n");
			fprintf(stderr, "eio_wdt pin <ms> : set watchdog pin timeout in milli-seconds\n\n");
			
			fprintf(stderr, "examples:\n");
			fprintf(stderr, "\teio_wdt dly 3000\n");
			fprintf(stderr, "\teio_wdt rst 2000\n");
			fprintf(stderr, "\teio_wdt start\n");
			fprintf(stderr, "\tThe reset event will be occurred after 5 seconds\n");
		}
	}
	else if (argc == 3)
	{
		wdt_type = 0;

		// dly : delay time
		// pwr : power cycle
		// irq : IRQ event
		// rst : Reset
		// pin : Watchdog Pin
		// sci : SCI event
		if 		(strcmp(argv[1], "dly") == 0)		wdt_type = 0x11;
		else if (strcmp(argv[1], "pwr") == 0)		wdt_type = 0x12;
		else if (strcmp(argv[1], "irq") == 0)		wdt_type = 0x13;
		else if (strcmp(argv[1], "rst") == 0)		wdt_type = 0x14;
		else if (strcmp(argv[1], "pin") == 0)		wdt_type = 0x15;
		else if (strcmp(argv[1], "sci") == 0)		wdt_type = 0x16;

		sscanf(argv[2], "%d", &wdt_tmo);

		if (wdt_type)
		{
			eio_wdt_set(wdt_type, wdt_tmo);
		}
		
		// watchdog example:
		// - delay time  = 3 seconds
		// - power cycle = 5 seconds
		
		// |---- dly -----|----- pwr ------|
		// 0s             3s               8s
	}
	else
	{
		eio_show_wdt();
	}

	eio_exit();

	return 0;
}
