#ifndef NOKIA_5110_H
#define NOKIA_5110_H

#include "hal_gpio.h"
#include "hal_spi.h"
#include "hal_delay.h"

#define SEGMETS_TOTAL (84)
#define BANKS_TOTAL (6)

#define SPI_PORT	GPIOA_BASE
#define CE_PORT		GPIO2
#define DC_PORT		GPIO3
#define RST_PORT	GPIO4
#define SCK_PORT	GPIO5
#define MOSI_PORT	GPIO7
#define LED_PORT	GPIO13

typedef struct {
	uint8_t id;
	uint8_t segments[SEGMETS_TOTAL];
} Bank;

typedef struct {
	Bank banks[BANKS_TOTAL];
	uint32_t curr;
} Memory;


void n5110_init(void);
void n5110_send_data(uint16_t byte);
void n5110_send_cmd(uint16_t cmd);
void n5110_cursor(int16_t x, int16_t y);
void n5510_clear_screen(void);

#endif /* NOKIA_5110_H */