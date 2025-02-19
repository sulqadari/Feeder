#include "nokia5110.h"

#define SLAVE_SELECT 	gpio_clear(SPI_PORT, CE_PIN)
#define SLAVE_DESELECT	gpio_set(GPIOB_BASE, RST_PIN)
/**
 * NOTE: due to inverted logic of the n5110 module, where LOW state designates
 * 'active' state, passing in 'true' puts the 'RST' line to low.
 * @param boolean true or false.
 */
static void
reset_module(bool mode)
{
	while (SPI1_IsBusy()) { }
	if (mode) {
		gpio_clear(GPIOB_BASE, RST_PIN);
	} else {
		gpio_set(GPIOB_BASE, RST_PIN);
	}
}

static void
send_byte(uint8_t byte, DataType type)
{
	if (type == TYPE_CMD) {
		while (SPI1_IsBusy()) { }
		gpio_clear(GPIOB_BASE, DC_PIN);
	}
	else {
		while (SPI1_IsBusy()) { }
		gpio_set(GPIOB_BASE, DC_PIN);
	}

	SPI1_Send(byte);
}

void
n5110_init(void)
{
	reset_module(true);
	reset_module(false);
	SLAVE_SELECT;
	send_byte(0x21, TYPE_CMD);	// Chip is active; use extended instruction set
	send_byte(0x14, TYPE_CMD);	// bias 1:40 (recommended)
	send_byte(0x04, TYPE_CMD);	// temp. coefficient '00' (recommended)
	
	send_byte(0x20, TYPE_CMD);	// horizontal addressing; use standard isntruction set;
	send_byte(0x09, TYPE_CMD);	// All display segments on
	send_byte(0x0C, TYPE_CMD);	// display normal mode
	SLAVE_DESELECT;
	n5510_clear_screen();
}

uint8_t
n5110_cursor(uint8_t x, uint8_t y)
{
	if (x > 83 || y > 5)
		return 1;

	SLAVE_SELECT;
	send_byte(0x40 | x, TYPE_CMD);
	send_byte(0x80 | y, TYPE_CMD);
	SLAVE_DESELECT;
	return 0;
}

void
n5510_clear_screen(void)
{
	n5110_cursor(0,0);
	SLAVE_SELECT;
	
	for (uint16_t i = 0; i < N5110_LCD_LENGTH; ++i)
		send_byte(0x00, TYPE_DATA);
	
	SLAVE_DESELECT;
}

void
n5510_black_screen(void)
{
	n5110_cursor(0,0);
	SLAVE_SELECT;

	for (uint16_t i = 0; i < N5110_LCD_LENGTH; ++i)
		send_byte(0xAA, TYPE_DATA);

	SLAVE_DESELECT;
}

void
n5510_print_logo(void)
{
	n5110_cursor(0,0);
	SLAVE_SELECT;

	for (uint16_t i = 0; i < sizeof(Telegram_Logo); ++i)
		send_byte(Telegram_Logo[i], TYPE_DATA);

	SLAVE_DESELECT;
}

// void
// n5510_put_char(char c)
// {
// 	for(uint8_t i = 0; i < 6; i++) {
// 		send_byte(ASCII[c - 0x20][i]);
// 	}
// }

// void
// n5510_print_string(char *str, uint8_t x, uint8_t y)
// {
// 	n5110_cursor(x, y);
// 	while(*str){
// 		n5510_put_char(*str++);
// 	}
// }