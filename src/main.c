#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_wdt.h"
#include "hal_spi.h"
#include "nokia5110.h"
#include "ugui_main.h"

typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
} Clock_t;

static void
increment(Clock_t* clock)
{
	clock->sec++;
	if (clock->sec >= 60) {
		clock->sec = 0;
		clock->min++;
		if (clock->min >= 60) {
			clock->min = 0;
			clock->hour++;
			if (clock->hour >= 24) {
				clock->hour = 0;
			}
		}
	}
}

static void
set_clock(Clock_t* clock, uint8_t h, uint8_t m, uint8_t s)
{
	clock->hour = h;
	clock->min = m;
	clock->sec = s;
}

int
main(void)
{
	Clock_t clock;
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
	n5110_set_cursor(0, 0);

	n5110_print_logo();
	DWT_delay_ms(1000);
	ugui_init(&ugui);

#if(0)
	memset(&clock, 0x00, sizeof(Clock_t));
	set_clock(&clock, 19, 35, 00);

	while (1) {
		n5110_set_cursor(LCD_WIDTH / 7, 2);
		increment(&clock);
		DWT_delay_ms(1000);
		gpio_toggle(GPIOC_BASE, LED_PIN);
		n5110_print_clock(clock.sec,clock.min, clock.hour);
	}

#else

	while (1) {

		gpio_toggle(GPIOC_BASE, LED_PIN);
		
				for (uint16_t r = 0; r < LCD_WIDTH / 2; ++r) {
					ugui_draw_circle( LCD_WIDTH / 2, LCD_HEIGHT / 2, r, C_BLACK);
					DWT_delay_ms(100);
				}
		
		for (uint16_t r = 0; r < LCD_WIDTH / 2; ++r) {
			ugui_draw_circle( LCD_WIDTH / 2, LCD_HEIGHT / 2, r, C_WHITE);
			DWT_delay_ms(100);
		}


		ugui_print_string(17, 14, "Eggplant");
		DWT_delay_ms(1000);
	}

#endif
	return 0;
}