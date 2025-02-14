#include "nokia5110.h"

static void
chip_enable(void)
{
	gpio_set(SPI_PORT, CE_PORT);
}

static void
chip_disable(void)
{
	gpio_clear(SPI_PORT, CE_PORT);
}

void
rst_enable(void)
{
	gpio_set(SPI_PORT, RST_PORT);
}

void
rst_disable(void)
{
	gpio_clear(SPI_PORT, RST_PORT);
}

/** The 'high' on the pin activates data mode */
static void
data_mode(void)
{
	gpio_set(SPI_PORT, DC_PORT);
}

/** The 'low' on the pin activates command mode */
static void
cmd_mode(void)
{
	gpio_clear(SPI_PORT, DC_PORT);
}

static void
check_spi(void)
{
	while (SPI1_IsBusy()) { }
}

void
n5110_init(void)
{
	chip_enable();
	rst_enable();

	chip_disable();
	rst_disable();
	cmd_mode();

	SPI1_Write(0x21);//exend COM
	SPI1_Write(0xc1);//voltage offset
	SPI1_Write(0x06);//temp. correction
	SPI1_Write(0x13);//high voltage(6.42V) generator on
	SPI1_Write(0x20);//standart COM
	SPI1_Write(0x0C);//LCD on

	check_spi();
	data_mode();
	n5110_cursor(0, 0);
}

void
n5110_send_data(uint16_t data)
{
	data_mode();
	SPI1_Write(data);
}

void
n5110_send_cmd(uint16_t cmd)
{
	cmd_mode();
	SPI1_Write(cmd);
}

void
n5110_cursor(int16_t x, int16_t y)
{
	if (x < 0 || x > 83)
		x = 0;
	
	if (y < 0 || y > 5)
		y = 0;
	
	n5110_send_cmd(0x40 | x);
	n5110_send_cmd(0x80 | y);
}

void
n5510_clear_screen(void)
{
	for (uint16_t i = 0; i < 504; ++i)
		n5110_send_data(0x00);
	
	n5110_cursor(0, 0);
}