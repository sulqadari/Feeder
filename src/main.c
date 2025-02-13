#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "hal_delay.h"

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

	while (1) {
		gpio_toggle(GPIOC_BASE, GPIO13);
		DWT_delay_ms(1000);
	}

	return 0;
}