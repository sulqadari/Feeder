#include "FreeRTOS.h"
#include "task.h"
// #include "semphr.h"

#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>

#include "mcuio.h"
#include "miniprintf.h"


static volatile uint32_t days = 0, hours = 0, minutes = 0, seconds = 0;
static TaskHandle_t task_1_Hdlr = 0;

void
RTC_IRQHandler(void)
{
	UBaseType_t intStatus;
	BaseType_t woken = pdFALSE;

	if (rtc_check_flag(RTC_SEC)) {
		rtc_clear_flag(RTC_SEC);

		intStatus = taskENTER_CRITICAL_FROM_ISR();
		if (++seconds > 59) {
			minutes++;
			seconds = 0;
		}

		if (++minutes > 59) {
			hours++;
			minutes = 0;
		}

		if (++hours > 24) {
			days++;
			hours = 0;
		}
		taskEXIT_CRITICAL_FROM_ISR(intStatus);

		vTaskNotifyGiveFromISR(task_1_Hdlr, &woken);	// Make task1 work again
		portYIELD_FROM_ISR(woken);
	}
}

static void
rtc_setup(void)
{
	rcc_enable_rtc_clock();
	rtc_interrupt_disable(RTC_SEC);
	// rtc_interrupt_disable(RTC_ALR);
	// rtc_interrupt_disable(RTC_OW);

	// RCC_HSE, RCC_LSE, RCC_LSI
	rtc_awake_from_off(RCC_HSE); 
	rtc_set_prescale_val(62500);
	rtc_set_counter_val(0xFFFFFFF0);

	nvic_enable_irq(NVIC_RTC_IRQ);

	cm_disable_interrupts();
	rtc_clear_flag(RTC_SEC);
	// rtc_clear_flag(RTC_ALR);
	// rtc_clear_flag(RTC_OW);
	rtc_interrupt_enable(RTC_SEC);
	// rtc_interrupt_enable(RTC_ALR);
	// rtc_interrupt_enable(RTC_OW);
	cm_enable_interrupts();
}

static void
task_1(void* args __attribute__((unused)))
{
	rtc_setup();

	for (;;) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);	// Block execution until notified
		gpio_toggle(GPIOC, GPIO13);	// Toggle LED
		
		std_printf("Time: %3lu days %02lu:%02lu:%02lu\n", days,hours,minutes,seconds);
	}
}

int
main(void)
{
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
	
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

	xTaskCreate(task_1, "task_1", 64, NULL, configMAX_PRIORITIES - 1, &task_1_Hdlr);

	gpio_clear(GPIOC, GPIO13);

	usb_start(1,1);
	std_set_device(mcu_usb);	// Use USB for std I/O

	vTaskStartScheduler();
	
	for (volatile uint8_t i = 0; ; ++i) {  }

	return 0;
}