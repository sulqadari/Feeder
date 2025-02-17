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
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_SPI1);

	gpio_set_mode(
		GPIOA_BASE,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		CE_PORT | DC_PORT | RST_PORT);	// SPI: CE | DC | RST

	gpio_set_mode(
		GPIOA_BASE,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		SCK_PORT | MOSI_PORT);			// SCK | MOSI

	gpio_set_mode(
		GPIOC_BASE,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		LED_PORT						// LED on port C13
	);

	SPI1_Init();
	n5110_init();
	DWT_delay_ms(250);
	n5510_clear_screen();
	DWT_delay_ms(250);
	uint16_t idx = 0;
	// while (1) {
	// 	gpio_toggle(GPIOC_BASE, LED_PORT);
	// 	DWT_delay_ms(250);
	// 	n5110_send_data(Telegram_Logo[idx++]);
		
	// 	if (idx > sizeof(Telegram_Logo) - 1) {
	// 		n5510_clear_screen();
	// 		idx = 0;
	// 	}
	// }
	for (uint8_t x = 0; x < 5; x++) {
		for (uint8_t y = 0; y < 5; y++) {
			gpio_toggle(GPIOC_BASE, LED_PORT);
			DWT_delay_ms(1000);
			n5110_cursor(y,x);
			n5110_send_data(0xFF);
		}
	}

	return 0;
}