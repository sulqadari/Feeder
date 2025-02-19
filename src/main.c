#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "hal_delay.h"
#include "hal_spi.h"
#include "nokia5110.h"

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
		LED_PIN	
	);

	SPI1_Init();
	n5110_init();
	n5510_print_logo();
	
	for (volatile int i = 0;;) {
		i++;
	}

	return 0;
}