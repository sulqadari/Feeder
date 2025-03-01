#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_wdt.h"
#include "hal_spi.h"
#include "lcd_main.h"

int
main(void)
{
	UG_GUI ugui;

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
	lcd_init(&ugui);
	n5110_set_cursor(0, 0);


	while (1) {

		gpio_toggle(GPIOC_BASE, LED_PIN);
		
		for (uint16_t r = 0; r < LCD_WIDTH / 2; ++r) {
			lcd_circle( LCD_WIDTH / 2, LCD_HEIGHT / 2, r, C_BLACK);
			DWT_delay_ms(100);
		}
		
		for (uint16_t r = 0; r < LCD_WIDTH / 2; ++r) {
			lcd_circle( LCD_WIDTH / 2, LCD_HEIGHT / 2, r, C_WHITE);
			DWT_delay_ms(100);
		}

		lcd_print_string(17, 14, "Eggplant");
		DWT_delay_ms(1000);
	}

	return 0;
}