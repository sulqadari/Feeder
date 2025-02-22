#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_wdt.h"
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
	
	while (1) {

		n5110_set_cursor(0, 0);
		n5110_fill_in(0x00);
		n5110_print_logo();
		
		DWT_delay_ms(5000);
		
		gpio_toggle(GPIOC_BASE, LED_PIN);
		
		n5110_set_cursor(0, 0);
		n5110_fill_in(0x00);
		n5110_set_cursor(0, 0);

		for (uint8_t x = 0; x < 6; ++x) {
			n5510_print_string("Freeder", x, 0);
			DWT_delay_ms(1000);
		}
	}

	return 0;
}