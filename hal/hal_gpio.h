#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <stdint.h>
#include "stm32f1xx.h"

#define MMIO32(addr)	(*(volatile uint32_t *)(addr))
#define GPIO_BSRR(port)	MMIO32((port) + 0x10)
#define GPIO_ODR(port)	MMIO32((port) + 0x0c)
#define GPIO_CRL(port)	MMIO32((port) + 0x00)
#define GPIO_CRH(port)	MMIO32((port) + 0x04)

#define GPIO0				(1 << 0)
#define GPIO1				(1 << 1)
#define GPIO2				(1 << 2)
#define GPIO3				(1 << 3)
#define GPIO4				(1 << 4)
#define GPIO5				(1 << 5)
#define GPIO6				(1 << 6)
#define GPIO7				(1 << 7)
#define GPIO8				(1 << 8)
#define GPIO9				(1 << 9)
#define GPIO10				(1 << 10)
#define GPIO11				(1 << 11)
#define GPIO12				(1 << 12)
#define GPIO13				(1 << 13)
#define GPIO14				(1 << 14)
#define GPIO15				(1 << 15)
#define GPIO_ALL			0xffff

#define GPIO_MODE_INPUT				0x00	/* Default */
#define GPIO_MODE_OUTPUT_10_MHZ		0x01
#define GPIO_MODE_OUTPUT_2_MHZ		0x02
#define GPIO_MODE_OUTPUT_50_MHZ		0x03

#define GPIO_CNF_INPUT_ANALOG		0x00
#define GPIO_CNF_INPUT_FLOAT		0x01	/** Digital Input Floating. Default */
#define GPIO_CNF_INPUT_PULL_UPDOWN	0x02		/** Digital Input Pull Up and Down */

/* CNF[1:0] values when MODE[1:0] is != 00 (one of the output modes) */
#define GPIO_CNF_OUTPUT_PUSHPULL	0x00		/** Digital Output Pushpull */
#define GPIO_CNF_OUTPUT_OPENDRAIN	0x01		/** Digital Output Open Drain */
#define GPIO_CNF_OUTPUT_ALTFN_PUSHPULL	0x02	/** Alternate Function Output Pushpull */
#define GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN	0x03	/** Alternate Function Output Open Drain */

void gpio_set_mode(uint32_t gpioport, uint8_t mode, uint8_t cnf, uint16_t gpios);
void gpio_toggle(uint32_t port, uint16_t gpios);
void gpio_set(uint32_t gpioport, uint16_t gpios);
void gpio_clear(uint32_t gpioport, uint16_t gpios);

#endif /* GPIO_HAL_H */