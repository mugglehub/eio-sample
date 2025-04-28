#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/types.h>

#include "../../eio/eio.h"

//=============================================================================
//  eio_fw_ver
//=============================================================================
void eio_fw_ver(void)
{
	uint8_t			res;
	eio_fw_prt_t	prt;
	uint32_t		*tmo;
	uint8_t			id[4];
	uint32_t		doc_ver;


	acpi_ec_read(0xFA, &id[0]);
	acpi_ec_read(0xFB, &id[1]);
	acpi_ec_read(0xFC, &id[2]);
	fprintf(stderr, "    ACPI Board ID : 0x%02X(%c) 0x%02X 0x%02X\n", id[0], id[0], id[1], id[2]);

	// development guide ver
	prt.cmd = 0x03;
	prt.ctl = 0x0F;
	prt.dev = 0;
	prt.len = 3;
	res = eio_fw_read_prt(&prt);

	doc_ver = prt.buf[2];
	doc_ver <<= 8;
	doc_ver |= prt.buf[1];
	doc_ver <<= 8;
	doc_ver |= prt.buf[0];

	if (!res)
		fprintf(stderr, "Develop Guide ver : %d.%d.%d = 0x%08X\n", prt.buf[2], prt.buf[1], prt.buf[0], doc_ver);
	else
		fprintf(stderr, "get develop guide ver err!\n");

	fprintf(stderr, "\n");

	// fw version
	prt.cmd = 0x53;
	prt.ctl = 0x22;
	prt.dev = 0;
	prt.len = 16;
	res = eio_fw_read_prt(&prt);

	if (!res)
		fprintf(stderr, "           FW ver : %s\n", prt.buf);
	else
		fprintf(stderr, "get FW ver err!\n");

	// fw version number
	prt.cmd = 0x53;
	prt.ctl = 0x21;
	prt.dev = 0;
	prt.len = 4;

	res = eio_fw_read_prt(&prt);
	tmo = (uint32_t *)prt.buf;
	printf("   Version Number : %d = 0x%X = %d\n", *tmo, *tmo, *tmo & 0xFFFF);

	// fw build time
	prt.cmd = 0x53;
	prt.ctl = 0x23;
	prt.dev = 0;
	prt.len = 26;
	res = eio_fw_read_prt(&prt);

	if (!res)
		fprintf(stderr, "    FW build time : %s\n", prt.buf);
	else
		fprintf(stderr, "get FW build time err!\n");

	// fw release date
	prt.cmd = 0x53;
	prt.ctl = 0x24;
	prt.dev = 0;
	prt.len = 16;
	res = eio_fw_read_prt(&prt);

	if (!res)
		fprintf(stderr, "  FW release date : %s\n", prt.buf);
	else
		fprintf(stderr, "get FW release date err!\n");

	fprintf(stderr, "\n");

	// board name
	prt.cmd = 0x53;
	prt.ctl = 0x10;
	prt.dev = 0;
	prt.len = 16;
	res = eio_fw_read_prt(&prt);

	if (!res)
		fprintf(stderr, "       Board Name : %s\n", prt.buf);
	else
		fprintf(stderr, "get board name err!\n");

	// board id
	prt.cmd = 0x53;
	prt.ctl = 0x1E;
	prt.dev = 0;
	prt.len = 4;
	res = eio_fw_read_prt(&prt);

	if (!res)
		fprintf(stderr, "         Board ID : %d = 0x%08X\n", *(uint32_t *)prt.buf, *(uint32_t *)prt.buf);
	else
		fprintf(stderr, "get board id err!\n");

	// board serial number
	prt.cmd = 0x53;
	prt.ctl = 0x1F;
	prt.dev = 0;
	prt.len = 16;
	res = eio_fw_read_prt(&prt);

	prt.buf[16] = 0;

	if (!res)
		fprintf(stderr, "        Board S/N : %s\n", prt.buf);
	else
		fprintf(stderr, "get board s/n err!\n");

	// manufacturer name
	prt.cmd = 0x53;
	prt.ctl = 0x11;
	prt.dev = 0;
	prt.len = 16;
	res = eio_fw_read_prt(&prt);

	if (!res)
		fprintf(stderr, "Manufacturer Name : %s\n", prt.buf);
	else
		fprintf(stderr, "get manufacturer name err!\n");

	// chip name
	prt.cmd = 0x53;
	prt.ctl = 0x12;
	prt.dev = 0;
	prt.len = 12;
	res = eio_fw_read_prt(&prt);

	if (!res)
		fprintf(stderr, "        Chip Name : %s\n", prt.buf);
	else
		fprintf(stderr, "get chip name err!\n");

	// platform type
	prt.cmd = 0x53;
	prt.ctl = 0x13;
	prt.dev = 0;
	prt.len = 16;
	res = eio_fw_read_prt(&prt);

	if (!res)
		fprintf(stderr, "    Platform Type : %s\n", prt.buf);
	else
		fprintf(stderr, "get platform type err!\n");

	// platform version
	prt.cmd = 0x53;
	prt.ctl = 0x14;
	prt.dev = 0;
	prt.len = 4;
	res = eio_fw_read_prt(&prt);

	if (!res)
		fprintf(stderr, "     Platform Ver : %d = 0x%08X\n", *(uint32_t *)prt.buf, *(uint32_t *)prt.buf);
	else
		fprintf(stderr, "get platform ver err!\n");

	// after 1.0.17
	if (doc_ver >= 0x010017)
	{
		// EAPI version
		prt.cmd = 0x53;
		prt.ctl = 0x30;
		prt.dev = 0;
		prt.len = 4;
		res = eio_fw_read_prt(&prt);

		if (!res)
			fprintf(stderr, "         EAPI Ver : %d = 0x%08X\n", *(uint32_t *)prt.buf, *(uint32_t *)prt.buf);
		else
			fprintf(stderr, "get eapi ver err!\n");

		// EAPI pnp id
		prt.cmd = 0x53;
		prt.ctl = 0x31;
		prt.dev = 0;
		prt.len = 4;
		res = eio_fw_read_prt(&prt);

		if (!res)
			fprintf(stderr, "          EAPI ID : %u = 0x%08X\n", *(uint32_t *)prt.buf, *(uint32_t *)prt.buf);
		else
			fprintf(stderr, "get eapi id err!\n");
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

	eio_fw_ver();

	eio_exit();

	return 0;
}
