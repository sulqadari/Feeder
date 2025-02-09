#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "hal_delay.h"
#include "lcd1602_i2c.h"

int
main(void)
{
	rcc_set_hse72();
	DWT_Init();
	rcc_periph_clock_enable(RCC_GPIOC);	// Enable clock on LED
	
	gpio_set_mode(
		GPIOC_BASE,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13			// LED on port C13
	);

	I2C1_Init(I2C_FAST);
	LCD1602_Init();
	
	while (1) {
		LCD1602_Led(LCD1602_ON);
		LCD1602_SendCommand(0x20);
		gpio_toggle(GPIOC_BASE, GPIO13);
		DWT_delay_ms(1000);
		LCD1602_Led(LCD1602_OFF);
		LCD1602_SendCommand(0x20);
		DWT_delay_ms(1000);
	}

	return 0;
}