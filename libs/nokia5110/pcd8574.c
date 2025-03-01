#include "pcd8574.h"
#include "hal_wdt.h"

static void
fill_in(uint8_t fill)
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
n5110_init(void)
{
	RESET_LOW;
	DWT_delay_ms(50);
	RESET_HIGH;

	NSS_LOW;
	CMD_MODE;
	SPI1_Send(FUNC_SET | FUNC_SET_EXT);
	SPI1_Send(OPERATION_VOLTAGE | (0x45 & 0x7F));
	SPI1_Send(TEMP_COEFF | 0x02);
	SPI1_Send(BIAS_SYSTEM | 0x03);

	SPI1_Send(FUNC_SET | FUNC_SET_BASIC);
	SPI1_Send(DIS_CONF_NORMAL);

	n5110_set_cursor(0, 0);
	fill_in(0x00);
	
	n5110_set_cursor(0, 0);

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
	
	SPI1_Send(0x80 | x);
	SPI1_Send(0x40 | y);
	
	while (SPI1_IsBusy());
	NSS_HIGH;
	return 0;
}

#if(0)

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

static void
put_char(char c)
{
	for(uint8_t i = 0; i < 6; i++) {
		SPI1_Send(ASCII[c - 0x20][i]);
	}
}

void
n5110_print_string(char *str, uint8_t x, uint8_t y)
{
	n5110_set_cursor(x, y);

	NSS_LOW;
	while (SPI1_IsBusy());
	DATA_MODE;

	while(*str){
		put_char(*str++);
	}

	while (SPI1_IsBusy());
	NSS_HIGH;
}

void
n5110_print_clock(uint8_t sec, uint8_t min, uint8_t hour)
{
	memset(str_buff, 0, sizeof(str_buff));

	mini_snprintf(str_buff, sizeof(str_buff), "%02d:%02d:%02d", hour, min, sec);

	NSS_LOW;
	while (SPI1_IsBusy());
	DATA_MODE;

	char* pBuff = str_buff;
	
	while(*pBuff) {
		put_char(*pBuff++);
	}

	while (SPI1_IsBusy());
	NSS_HIGH;
}
#endif