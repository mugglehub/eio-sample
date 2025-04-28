#ifndef __EIO_H
#define __EIO_H

//=============================================================================
//  ACPI
//=============================================================================
#define ACPI_CMD_PORT	0x66
#define ACPI_STS_PORT	0x66
#define ACPI_DAT_PORT	0x62

#define ACPI_CMD_READ	0x80
#define ACPI_CMD_WRITE	0x81

#define ACPI_STS_OBF	0x01
#define ACPI_STS_IBF	0x02

#define ACPI_RETRIES	(1000)

#define ACPI_OK			0
#define ACPI_ERR_OBF	1
#define ACPI_ERR_IBE	2
#define ACPI_CHECK_RESULT(res)	{ if (res != ACPI_OK) return res; }


//=============================================================================
//  EIO
//=============================================================================
#define EIO_RETRIES		(1000)
#define EIO_CMD_PORT	0x2F6
#define EIO_STS_PORT	0x2F6
#define EIO_DAT_PORT	0x2F2

#define EIO_CFG_IDX		0x299
#define EIO_CFG_DAT		0x29A

#define EIO_OBF			0x1
#define EIO_IBF			0x2
#define EIO_IBF_OBF		0x3

#define EIO_FW_BUF_SZ	(64)

#define EIO_LDN_GPIO	0x24


// number of devices
#define EIO_MAX_CASEOPEN	(1)
#define EIO_MAX_CURR		(2)
#define EIO_MAX_FAN			(4)
#define EIO_MAX_PANEL		(2)
#define EIO_MAX_PWM			(3)
#define EIO_MAX_TACH		(2)
#define EIO_MAX_THERM		(4)
#define EIO_MAX_VOLT		(8)


// error code
#define EIO_OK			0
#define EIO_ERR			1

//=============================================================================
//  eio_fw_prt_t : EIO f/w protocol
//=============================================================================
typedef struct _eio_fw_prt_t
{
	uint8_t		cmd;
	uint8_t		ctl;
	uint8_t		dev;
	uint8_t		len;
	uint8_t		buf[EIO_FW_BUF_SZ];

} eio_fw_prt_t;

//=============================================================================
//  variables
//=============================================================================
extern uint8_t	eio_gpio_irq;
extern uint16_t	eio_gpio_base;

//=============================================================================
//  functions
//=============================================================================
uint8_t acpi_ec_read(uint8_t idx, uint8_t *datap);
uint8_t acpi_ec_write(uint8_t idx, uint8_t data);
uint8_t eio_fw_read_prt(eio_fw_prt_t *prt);
uint8_t eio_fw_write_prt(eio_fw_prt_t *prt);

uint8_t	eio_gpio_init(void);
void	eio_gpio_exit(void);
uint8_t	eio_init(void);
void	eio_exit(void);

#endif