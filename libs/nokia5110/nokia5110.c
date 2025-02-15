#include "nokia5110.h"

/** @param either MODE_SET or MODE_RESET */
static void
reset_mode(uint8_t mode)
{
	if (mode)
		gpio_set(SPI_PORT, RST_PORT);
	else
		gpio_clear(SPI_PORT, RST_PORT);
}

/** @param either MODE_SET or MODE_RESET */
static void
chip_mode(uint8_t mode)
{
	if (mode)
		gpio_set(SPI_PORT, CE_PORT);
	else
		gpio_clear(SPI_PORT, CE_PORT);
}

/** @param mode: either MODE_DATA or MODE_CMD */
static void
dc_mode(uint8_t mode)
{
	if (mode)
		gpio_set(SPI_PORT, DC_PORT);
	else
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
	chip_mode(MODE_SET);
	dc_mode(MODE_DATA);
	reset_mode(MODE_SET);
	
	chip_mode(MODE_RESET);
	reset_mode(MODE_RESET);
	dc_mode(MODE_CMD);


	SPI1_Write(0x21);	// exend commands mode
	SPI1_Write(0xC1);	// contrast
	SPI1_Write(0x04);	// temp. coefficient (recommended)
	SPI1_Write(0x14);	// bias 1:40 (recommended)
	SPI1_Write(0x20);	// standard commands and horizontal mode
	SPI1_Write(0x0C);	// LCD normal

	check_spi();
	dc_mode(MODE_DATA);
}

void
n5110_send_data(uint16_t data)
{
	dc_mode(MODE_DATA);
	SPI1_Write(data);
}

void
n5110_send_cmd(uint16_t cmd)
{
	dc_mode(MODE_CMD);
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
	n5110_cursor(0, 0);
	for (uint16_t i = 0; i < 6 * 84; ++i)
		n5110_send_data(0x00);
	
	n5110_cursor(0, 0);
}

void
n5510_put_char(char c)
{
	for(uint8_t i = 0; i < 6; i++) {
		n5110_send_data(ASCII[c - 0x20][i]);
	}
}

void
n5510_print_string(char *str, uint8_t x, uint8_t y)
{
	n5110_cursor(x, y);
	while(*str){
		n5510_put_char(*str++);
	}
  }