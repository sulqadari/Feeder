#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_rcc.h"
#include "hal_gpio.h"

int
main(void)
{
	rcc_set_hse72();

	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC_BASE,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13);

	while (1) {
		gpio_toggle(GPIOC_BASE, GPIO13);
		for (volatile uint32_t i = 0; i < 1000000; ++i) {  }

	}

	return 0;
}