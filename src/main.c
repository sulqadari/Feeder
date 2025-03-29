#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_wdt.h"
#include "hal_spi.h"
#include "lcd_main.h"
#include "hal_exti.h"
#include "miniprintf.h"

#define LED_PIN		GPIO13

static void
LED_Init(void)
{
    rcc_periph_clock_enable(RCC_GPIOC);	// Enable clock on LED
	gpio_set_mode(
		GPIOC_BASE,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		LED_PIN	
	);
}

int
main(void)
{
	UG_GUI ugui;
	uint32_t press_count = 0;

	rcc_set_hse72();
	DWT_Init();

    LED_Init();
	SPI1_Init();
	lcd_init(&ugui);
    EXTI_Init();

	while (1) {
#if (1)
        if (left_btn) {
			lcd_clear_pixmap();
			mini_snprintf(printf_array, PRINTF_ARRAY_LEN, "button pressed\ncount %d", press_count++);
            lcd_print_string(0, 0, printf_array);
            left_btn = 0;
        }
#else
		gpio_toggle(GPIOC_BASE, LED_PIN);
				
		for (uint16_t r = 0; r < LCD_WIDTH / 2; ++r) {
			lcd_circle( LCD_WIDTH / 2, LCD_HEIGHT / 2, r, C_BLACK);
			DWT_delay_ms(50);
		}

		for (uint16_t r = 0; r < LCD_WIDTH / 2; ++r) {
			lcd_circle( LCD_WIDTH / 2, LCD_HEIGHT / 2, r, C_WHITE);
			DWT_delay_ms(50);
		}
#endif
    }

	return 0;
}