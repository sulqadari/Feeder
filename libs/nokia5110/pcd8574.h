#ifndef NOKIA_H5110_H
#define NOKIA_H5110_H

#include "hal_spi.h"
#include "hal_gpio.h"
#include "logos.h"

#define DATA_TYPE_CMD	0x00
#define DATA_TYPE_DATA	0x01

#define LCD_WIDTH		84
#define LCD_HEIGHT		48

#define LCD_COLUMNS_TOTAL   LCD_WIDTH
#define LCD_ROWS_TOTAL      6
#define LCD_BUFFER_SIZE	((LCD_WIDTH * LCD_HEIGHT) / 8)

#define NSS_HIGH	gpio_set(GPIOB_BASE, SE_PIN)
#define NSS_LOW		gpio_clear(GPIOB_BASE, SE_PIN)

#define DATA_MODE	gpio_set(GPIOB_BASE, DC_PIN)
#define CMD_MODE	gpio_clear(GPIOB_BASE, DC_PIN)

#define RESET_HIGH	gpio_set(GPIOB_BASE, RST_PIN)
#define RESET_LOW	gpio_clear(GPIOB_BASE, RST_PIN)

#define FUNC_SET		0x20
#define FUNC_SET_BASIC	0x00
#define FUNC_SET_EXT	0x01	/*!< Extended functions set */
#define FUNC_SET_VADDR	0x02	/*!< Vertical addressing */
#define FUNC_SET_PDMODE	0x04	/*!< Power-down mode */

#define EXT_TEMP_CTRL		0x40	/*!< Extended functions set */
#define EXT_TEMP_CTRL_C0	0x00
#define EXT_TEMP_CTRL_C1	0x01
#define EXT_TEMP_CTRL_C2	0x02
#define EXT_TEMP_CTRL_C3	0x03

#define DIS_CONF_BLANK		0x08
#define DIS_CONF_NORMAL		0x0C
#define DIS_CONF_SEGS_ON	0x09
#define DIS_CONF_INVERSE	0x0D

#define OPERATION_VOLTAGE	0x80	/*<! Affects the brightness of the pixels. */
#define TEMP_COEFF			0x04
#define BIAS_SYSTEM			0x10

void n5110_init(void);
uint8_t n5110_set_cursor(uint8_t x, uint8_t y);

#endif /* NOKIA_H5110_H */