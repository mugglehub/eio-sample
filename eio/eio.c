#include <stdio.h>
#include <stdint.h>
#include <sys/io.h>
#include <sys/types.h>

#include "eio.h"


//=============================================================================
//  variables
//=============================================================================
uint8_t		eio_gpio_irq = 0;
uint16_t	eio_gpio_base = 0;

//=============================================================================
//  eio_get_ldn
//=============================================================================
void eio_get_ldn(uint8_t ldn, uint8_t *en, uint8_t *irq, uint16_t *base)
{
	// ldn : logical device number
	
	// enter cfg
	outb(0x87, EIO_CFG_IDX);
	outb(0x87, EIO_CFG_IDX);

	outb(0x07, EIO_CFG_IDX);
	outb(ldn,  EIO_CFG_DAT);

	outb(0x30, EIO_CFG_IDX);
	*en = inb(EIO_CFG_DAT);

	outb(0x70, EIO_CFG_IDX);
	*irq = inb(EIO_CFG_DAT);

	outb(0x60, EIO_CFG_IDX);
	*base = inb(EIO_CFG_DAT);
	*base <<= 8;
	outb(0x61, EIO_CFG_IDX);
	*base |= inb(EIO_CFG_DAT);

	// exit cfg
	outb(0xAA, EIO_CFG_IDX);
}

//=============================================================================
//  acpi_wait_ibe
//=============================================================================
static uint8_t acpi_wait_ibe(void)
{
	uint16_t	retries = ACPI_RETRIES;

	while (inb(ACPI_STS_PORT) & ACPI_STS_IBF)
	{
		if (--retries == 0)
			return ACPI_ERR_IBE;
	}

	return ACPI_OK;
}

//=============================================================================
//  acpi_wait_obf
//=============================================================================
static uint8_t acpi_wait_obf(void)
{
	uint16_t	retries = ACPI_RETRIES;

	while ((inb(ACPI_STS_PORT) & ACPI_STS_OBF) == 0)
	{
		if (--retries == 0)
			return ACPI_ERR_OBF;
	}

	return ACPI_OK;
}

//=============================================================================
//  acpi_ec_read
//=============================================================================
uint8_t acpi_ec_read(uint8_t idx, uint8_t *datap)
{
	uint8_t		res = 0;

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(ACPI_CMD_READ, ACPI_CMD_PORT);

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(idx, ACPI_DAT_PORT);

	res = acpi_wait_obf();
	ACPI_CHECK_RESULT(res);

	*datap = inb(ACPI_DAT_PORT);

	return res;
}

//=============================================================================
//  acpi_ec_write
//=============================================================================
uint8_t acpi_ec_write(uint8_t idx, uint8_t data)
{
	uint8_t		res = 0;

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(ACPI_CMD_WRITE, ACPI_CMD_PORT);

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(idx, ACPI_DAT_PORT);

	res = acpi_wait_ibe();
	ACPI_CHECK_RESULT(res);

	outb(data, ACPI_DAT_PORT);

	return res;
}

//=============================================================================
//  eio_wait_ibe
//=============================================================================
static uint8_t eio_wait_ibe(void)
{
	uint16_t	retries = EIO_RETRIES;

	do
	{
		if ((inb(EIO_STS_PORT) & EIO_IBF) == 0)
			break;

	} while (retries--);

	if (retries)
		return 0;
	return 0xFF;
}

//=============================================================================
//  eio_wait_obf
//=============================================================================
static uint8_t eio_wait_obf(void)
{
	uint16_t	retries = EIO_RETRIES;

	do
	{
		if (inb(EIO_STS_PORT) & EIO_OBF)
			break;

	} while (retries--);

	if (retries)
		return 0;
	return 0xFF;
}

//=============================================================================
//  eio_wait_obe
//=============================================================================
static uint8_t eio_wait_obe(void)
{
	uint16_t	retries = EIO_RETRIES;

	do
	{
		if ((inb(EIO_STS_PORT) & EIO_OBF) == 0)
			break;

		// flush data port
		inb(EIO_DAT_PORT);

	} while (retries--);

	if (retries)
		return 0;
	return 0xFF;
}

//=============================================================================
//  eio_fw_read_prt
//=============================================================================
uint8_t eio_fw_read_prt(eio_fw_prt_t *prt)
{
	uint8_t		res;
	uint8_t		*datap;
	int			i;

	if (eio_wait_ibe())
		return 0xFF;
	outb(prt->cmd, EIO_CMD_PORT);

	if (eio_wait_ibe())
		return 0xFF;
	outb(prt->ctl, EIO_DAT_PORT);

	if (eio_wait_ibe())
		return 0xFF;
	outb(prt->dev, EIO_DAT_PORT);

	if (eio_wait_ibe())
		return 0xFF;
	outb(prt->len, EIO_DAT_PORT);

	datap = prt->buf;

	for (i=0; i<prt->len; i++)
	{
		if (eio_wait_obf())
			return 0xFF;

		*datap++ = inb(EIO_DAT_PORT);
	}

	return 0;
}

//=============================================================================
//  eio_fw_write_prt
//=============================================================================
uint8_t eio_fw_write_prt(eio_fw_prt_t *prt)
{
	uint8_t		res;
	uint8_t		*datap;
	int			i;

	if (eio_wait_ibe())
		return 0xFF;
	outb(prt->cmd, EIO_CMD_PORT);

	if (eio_wait_ibe())
		return 0xFF;
	outb(prt->ctl, EIO_DAT_PORT);

	if (eio_wait_ibe())
		return 0xFF;
	outb(prt->dev, EIO_DAT_PORT);

	if (eio_wait_ibe())
		return 0xFF;
	outb(prt->len, EIO_DAT_PORT);

	datap = prt->buf;

	for (i=0; i<prt->len; i++)
	{
		if (eio_wait_ibe())
			return 0xFF;
		outb(*datap++, EIO_DAT_PORT);
	}

	return 0;
}

//=============================================================================
//  eio_gpio_init
//=============================================================================
uint8_t eio_gpio_init(void)
{
	uint8_t		en;
	uint8_t		irq;
	uint16_t	base;
	
	eio_get_ldn(EIO_LDN_GPIO, &en, &irq, &base);
	
	if (en != 0x01)
		return 0xFF;	// device is disabled

	if (ioperm(base, 8, 1))
	{
		perror("ioperm port gpio");
		return 0xFE;
	}

	eio_gpio_irq	= irq;
	eio_gpio_base	= base;

	return 0;
}

//=============================================================================
//  eio_gpio_exit
//=============================================================================
void eio_gpio_exit(void)
{
	if (eio_gpio_base)
	{
		if (ioperm(eio_gpio_base, 8, 0))
			perror("ioperm port gpio");
	}
}

//=============================================================================
//  eio_init
//=============================================================================
uint8_t eio_init(void)
{
	uint8_t		lsb;
	uint8_t		msb;
	uint16_t	chipid;

	if (ioperm(0x299, 2, 1))
	{
		perror("ioperm port 299h");
		return 0xFF;
	}
	
	if (ioperm(0x2F0, 8, 1))
	{
		perror("ioperm port 2F0h");
		return 0xFE;
	}

	if (ioperm(0x62, 1, 1))
	{
		perror("ioperm port 62h");
		return 0xFD;
	}

	if (ioperm(0x66, 1, 1))
	{
		perror("ioperm port 66h");
		return 0xFC;
	}

	// enter cfg
	outb(0x87, EIO_CFG_IDX);
	outb(0x87, EIO_CFG_IDX);

	// chip id
	outb(0x20, EIO_CFG_IDX);
	msb = inb(EIO_CFG_DAT);

	outb(0x21, EIO_CFG_IDX);
	lsb = inb(EIO_CFG_DAT);

	chipid = (uint16_t)msb;
	chipid <<= 8;
	chipid |= (uint16_t)lsb;

	// exit cfg
	outb(0xAA, EIO_CFG_IDX);

	if (chipid != EIO_CHIPID_IS200 && chipid != EIO_CHIPID_201)
	{
		eio_exit();
		return EIO_ERR_CHIPID;
	}

	return EIO_OK;
}

//=============================================================================
//  eio_exit
//=============================================================================
void eio_exit(void)
{
	if (ioperm(0x299, 2, 0))
		perror("ioperm port 299h");
	
	if (ioperm(0x2F0, 8, 0))
		perror("ioperm port 2F0h");

	if (ioperm(0x62, 1, 0))
		perror("ioperm port 62h");

	if (ioperm(0x66, 1, 0))
		perror("ioperm port 66h");
}
