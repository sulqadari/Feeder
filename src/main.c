#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_rcc.h"
#include "hal_gpio.h"
#include "hal_wdt.h"
#include "hal_spi.h"
#include "lcd_main.h"
#include "hal_exti.h"
#include "hal_rtc.h"
#include "flashmngr.h"

#include "miniprintf.h"

#define LED_PIN		GPIO13

extern uint32_t test_counter;

static void
LED_Init(void)
{
    RCC_Periph_clock_en(RCC_GPIOC);	// Enable clock on LED
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
	uint32_t press_count[4] = {0};

	RCC_Init_HSE72();
	DWT_Init();

    LED_Init();
	SPI2_Init();
	lcd_init(&ugui);
    
	EXTI_Init();
	RTC_Init();

    mmInit();
	while (1) {
		switch (which_one) {
			case 0: {
				lcd_clear_pixmap();
				mini_snprintf(printf_array, PRINTF_ARRAY_LEN, "param 'A+'\n%d", ++press_count[0]);
				lcd_print_string(0, 0, printf_array);
				which_one = 0xff;
			} break;
			case 1: {
				lcd_clear_pixmap();
				mini_snprintf(printf_array, PRINTF_ARRAY_LEN, "param 'A-'\n%d", --press_count[0]);
				lcd_print_string(0, 0, printf_array);
				which_one = 0xff;
			} break;
			case 2: {
                uint32_t test_prev = 0;
				which_one = 0xff;
				
				while (1) {
					if (test_prev < test_counter) {
						test_prev = test_counter;
						lcd_clear_pixmap();
						mini_snprintf(printf_array, PRINTF_ARRAY_LEN, "test counter'\nvalue: %d", test_counter);
						lcd_print_string(0, 0, printf_array);
					}

					if (which_one != 0xff) {
						break;
					}
				}
				
			} break;
			case 3: {
				lcd_clear_pixmap();
				mini_snprintf(printf_array, PRINTF_ARRAY_LEN, "param 'B-'\n%d", --press_count[1]);
				lcd_print_string(0, 0, printf_array);
				which_one = 0xff;
			} break;
			default: /* Just skip this iteration. */
		}
    }

	return 0;
}