#include "nokia5110.h"

/**
 * @param either SET or RESET
 */
static void
set_reset(uint8_t mode)
{
	while (SPI1_IsBusy()) { }
	if (mode)
		gpio_set(SPI_PORT, RST_PIN);
	else
		gpio_clear(SPI_PORT, RST_PIN);
}

/**
 * @param mode: either TYPE_DATA or TYPE_CMD
 */
static void
set_data_type(uint8_t mode)
{
	if (mode)
		gpio_set(SPI_PORT, DC_PIN);
	else
		gpio_clear(SPI_PORT, DC_PIN);
}
	
/**
 * @param either SET or RESET
 */
static void
chip_enable_disable(uint8_t mode)
{
	if (mode)
		gpio_set(SPI_PORT, CE_PIN);
	else
		gpio_clear(SPI_PORT, CE_PIN);
}


void
n5110_init(void)
{
	set_reset(RESET);
	set_reset(SET);
	n5110_send_cmd(0x21);	// Chip is active; use extended instruction set
	n5110_send_cmd(0x04);	// temp. coefficient '00' (recommended)
	n5110_send_cmd(0x14);	// bias 1:40 (recommended)
	
	n5110_send_cmd(0x20);	// horizontal addressing; use standard isntruction set;
	n5110_send_cmd(0x09);	// All display segments on
	n5110_send_cmd(0x0C);	// display normal mode
}

void
n5110_send_cmd(uint8_t byte)
{
	set_data_type(TYPE_CMD);
	chip_enable_disable(RESET);
	SPI1_Send(byte);
	while (SPI1_IsBusy()) { }
	chip_enable_disable(SET);
}

void
n5110_send_data(uint8_t byte)
{
	set_data_type(TYPE_DATA);
	chip_enable_disable(RESET);
	SPI1_Send(byte);
	while (SPI1_IsBusy()) { }
	chip_enable_disable(SET);
}

uint8_t
n5110_cursor(uint8_t x, uint8_t y)
{
	if (x > 83 || y > 5)
		return 0;

	n5110_send_cmd(0x40 | x);
	n5110_send_cmd(0x80 | y);
	return 0;
}

void
n5510_clear_screen(void)
{
	n5110_cursor(0,0);
	for (uint16_t i = 0; i < 6 * 84; ++i)
		n5110_send_data(0x00);
}

void
n5510_black_screen(void)
{
	n5110_cursor(0,0);
	for (uint16_t i = 0; i < 6 * 84; ++i)
		n5110_send_data(0xFF);
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