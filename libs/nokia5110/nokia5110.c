#include "nokia5110.h"
#include "hal_wdt.h"

void
n5110_init(void)
{
	NSS_HIGH;
	DATA_MODE;
	RESET_HIGH;

	NSS_LOW;
	RESET_LOW;
	CMD_MODE;

	SPI1_Send(0x21);
	SPI1_Send(0xC1);
	SPI1_Send(0x06);
	SPI1_Send(0x13);
	SPI1_Send(0x20);
	SPI1_Send(0x0C);

	n5110_set_cursor(0, 0);
	n5110_fill_in(0x00);
	DWT_delay_ms(1000);

	n5110_set_cursor(0, 0);
	n5110_fill_in(0xaa);
	DWT_delay_ms(1000);
	
	n5110_set_cursor(0, 0);
	n5110_print_logo();

	DWT_delay_ms(1000);
	n5110_fill_in(0x00);

	while (SPI1_IsBusy());
	NSS_HIGH;
}

void
n5110_send(uint8_t data, uint8_t type)
{
	NSS_LOW;
	while (SPI1_IsBusy());

	if (type == DATA_TYPE_DATA)
		DATA_MODE;
	else
		CMD_MODE;
	
	SPI1_Send(data);
	while (SPI1_IsBusy());
	NSS_HIGH;
}

void
n5110_fill_in(uint8_t fill)
{
	NSS_LOW;
	while (SPI1_IsBusy());
	DATA_MODE;

	for (uint16_t i = 0; i < LCD_BUFFER_SIZE; ++i) {
		SPI1_Send(fill);
	}

	while (SPI1_IsBusy());
	NSS_HIGH;
}

void
n5110_print_logo(void)
{
	NSS_LOW;
	while (SPI1_IsBusy());
	DATA_MODE;

	for (uint16_t i = 0; i < LCD_BUFFER_SIZE; ++i) {
		SPI1_Send(Telegram_Logo[i]);
	}

	while (SPI1_IsBusy());
	NSS_HIGH;
}

uint8_t
n5110_set_cursor(uint8_t x, uint8_t y)
{
	if (x >= LCD_WIDTH) {
		return 1;
	} else if (y >= LCD_HEIGHT) {
		return 2;
	}
	
	NSS_LOW;
	while (SPI1_IsBusy());
	CMD_MODE;

	SPI1_Send(0x40 | x);
	SPI1_Send(0x80 | y);

	while (SPI1_IsBusy());
	NSS_HIGH;
	return 0;
}

uint8_t
n5110_test(uint8_t value, uint8_t x, uint8_t y)
{
	n5110_set_cursor(x, y);

	NSS_LOW;
	n5110_send(value, DATA_TYPE_DATA);

	while (SPI1_IsBusy());
	NSS_HIGH;
	return 0;
}

void
n5510_put_char(char c)
{
	for(uint8_t i = 0; i < 6; i++) {
		SPI1_Send(ASCII[c - 0x20][i]);
	}
}

void
n5510_print_string(char *str, uint8_t x, uint8_t y)
{
	n5110_set_cursor(x, y);

	NSS_LOW;
	while (SPI1_IsBusy());
	DATA_MODE;

	while(*str){
		n5510_put_char(*str++);
	}

	while (SPI1_IsBusy());
	NSS_HIGH;
}