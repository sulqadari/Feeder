#ifndef NOKIA_H5110_H
#define NOKIA_H5110_H

#include "hal_spi.h"
#include "hal_gpio.h"
#include "logos.h"
#include "font.h"

#define DATA_TYPE_CMD	0x00
#define DATA_TYPE_DATA	0x01

#define LCD_WIDTH		84
#define LCD_HEIGHT		48
#define LCD_BUFFER_SIZE	((LCD_WIDTH * LCD_HEIGHT) / 8)

#define NSS_HIGH	gpio_set(GPIOA_BASE, SE_PIN)
#define NSS_LOW		gpio_clear(GPIOA_BASE, SE_PIN)

#define DATA_MODE	gpio_set(GPIOA_BASE, DC_PIN)
#define CMD_MODE	gpio_clear(GPIOA_BASE, DC_PIN)

#define RESET_HIGH	gpio_set(GPIOA_BASE, RST_PIN)
#define RESET_LOW	gpio_clear(GPIOA_BASE, RST_PIN)

void n5110_init(void);
void n5110_send(uint8_t data, uint8_t type);
void n5110_fill_in(uint8_t fill);
void n5110_print_logo(void);
uint8_t n5110_set_cursor(uint8_t x, uint8_t y);
void n5510_put_char(char c);
void n5510_print_string(char *str, uint8_t x, uint8_t y);

#endif /* NOKIA_H5110_H */