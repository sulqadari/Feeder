#include "nokia5110.h"

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
	SPI1_Send(0x14);
	SPI1_Send(0xB6);
	SPI1_Send(0x04);
	SPI1_Send(0x20);
	SPI1_Send(0x0C);

	n5110_fill_in(0x00);
	DWT_delay_ms(1000);
	n5110_fill_in(0xaa);
}

void
n5110_send(uint8_t data, uint8_t type)
{
	if (type == DATA_TYPE_DATA)
		DATA_MODE;
	else
		CMD_MODE;
	
	SPI1_Send(data);
}

void
n5110_fill_in(uint8_t fill)
{
	while (SPI1_IsBusy());
	DATA_MODE;
	NSS_LOW;

	for (uint16_t i = 0; i < LCD_BUFFER_SIZE; ++i) {
		SPI1_Send(fill);
	}
	NSS_HIGH;
}