#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/i2c.h>

typedef enum {
	I2C_Ok = 0,
	I2C_Addr_Timeout,
	I2C_Addr_NAK,
	I2C_Write_Timeout,
	I2C_Read_Timeout
} I2C_Fails;

enum I2C_RW {
	Read = 1,
	Write = 0
};

typedef struct {
	uint32_t	device;		// I2C device
	uint32_t	timeout;	// Ticks
} I2C_Control;

typedef struct {
	uint32_t days;
	uint32_t hours;
	uint32_t minutes;
	uint32_t seconds;
} Calendar;

static Calendar* clndr;
static I2C_Control i2c;			// I2C Control struct

#define systicks			xTaskGetTickCount
#define PCF8574_ADDR(n)		(0x20 | ((n) & 0x07))	// PCF8574

static void
initCalendar(Calendar* calendar)
{
	calendar->days = 0;
	calendar->hours = 0;
	calendar->minutes = 0;
	calendar->seconds = 0;
	clndr = calendar;
}

static TaskHandle_t LEDtaskHdlr = 0;

void
RTC_IRQHandler(void)
{
	UBaseType_t intStatus;
	BaseType_t woken = pdFALSE;

	if (rtc_check_flag(RTC_SEC)) {
		rtc_clear_flag(RTC_SEC);

		intStatus = taskENTER_CRITICAL_FROM_ISR();
		if (++clndr->seconds > 59) {
			clndr->minutes++;
			clndr->seconds = 0;
		}

		if (++clndr->minutes > 59) {
			clndr->hours++;
			clndr->minutes = 0;
		}

		if (++clndr->hours > 24) {
			clndr->days++;
			clndr->hours = 0;
		}
		taskEXIT_CRITICAL_FROM_ISR(intStatus);

		vTaskNotifyGiveFromISR(LEDtaskHdlr, &woken);	// Make task1 work again
		portYIELD_FROM_ISR(woken);
	}
}

static void
i2c_write(I2C_Control* dev, uint8_t byte)
{
	i2c_send_data(dev->device, byte);
	while(!(I2C_SR1(dev->device) & (I2C_SR1_BTF))) {
		taskYIELD();
	}
}

/** Passes over the CPU to another task if I2C interface is busy. */
static void
i2c_wait_busy(I2C_Control* dev)
{
	while(I2C_SR2(dev->device) & I2C_SR2_BUSY) {
		taskYIELD();
	}
}

static void
i2c_start_addr(I2C_Control* dev, uint8_t addr, enum I2C_RW mode)
{
	// TickType_t t0 = systicks();

	i2c_wait_busy(dev);						// Block until not busy.
	I2C_SR1(dev->device) &= ~I2C_SR1_AF;	// Clear Acknowledge failure
	i2c_clear_stop(dev->device);			// Don't generate a stop

	if (mode == Read )
		i2c_enable_ack(dev->device);
	
	i2c_send_start(dev->device);			// Generate a Start/Restart

	// Loop until ready
	while(!((I2C_SR1(dev->device) & I2C_SR1_SB)
		&& (I2C_SR2(dev->device) & (I2C_SR2_MSL | I2C_SR2_BUSY)))) {
		taskYIELD();
	}

	i2c_send_7bit_address(dev->device, addr, mode == Read ? I2C_READ : I2C_WRITE);
	// t0 = systicks();

	while(!(I2C_SR1(dev->device) & I2C_SR1_ADDR)) {
		taskYIELD();
	}

	(void)I2C_SR2(dev->device);				// Clear flags
}

static void
i2c_stop(I2C_Control *dev)
{
	i2c_send_stop(dev->device);
}

static void
i2c_reset(uint32_t i2c)
{
	switch(i2c) {
		case I2C1: rcc_periph_reset_pulse(RST_I2C1); break;
		case I2C2: rcc_periph_reset_pulse(RST_I2C2); break;
		default: break;
	}
}

static void
i2c_configure(I2C_Control *dev, uint32_t i2c, uint32_t ticks)
{
	dev->device = i2c;
	dev->timeout = ticks;

	i2c_peripheral_disable(dev->device);
	i2c_reset(dev->device);
	
	I2C_CR1(dev->device) &= ~I2C_CR1_STOP;
	i2c_set_standard_mode(dev->device);			// 100 KHz mode
	
	i2c_set_clock_frequency(dev->device, 36);	// APB Frequency
	i2c_set_trise(dev->device, 36);				// 1000 ns
	
	i2c_set_dutycycle(dev->device, I2C_CCR_DUTY_DIV2);
	i2c_set_ccr(dev->device, 180);				// 100 KHz <= 180 * 1 / 36M

	i2c_peripheral_enable(dev->device);

}

static void
rtc_setup(void)
{
	rcc_enable_rtc_clock();
	rtc_interrupt_disable(RTC_SEC);
	
	rtc_awake_from_off(RCC_LSE); 
	rtc_set_prescale_val(0xFFFF);
	rtc_set_counter_val(0x0);

	nvic_enable_irq(NVIC_RTC_IRQ);
	cm_disable_interrupts();
	rtc_clear_flag(RTC_SEC);
	
	rtc_interrupt_enable(RTC_SEC);
	cm_enable_interrupts();
}

static void
thread_clock(void* args __attribute__((unused)))
{
	uint8_t addr = PCF8574_ADDR(0);
	uint8_t value = 0x30;

	i2c_configure(&i2c, I2C1, 1000);

	for (;;) {
		i2c_start_addr(&i2c, addr, Write);
		i2c_write(&i2c, value);
		i2c_stop(&i2c);
		value = (value + 1) % 10;
	}
}

static void
thread_LED(void* atgs __attribute__((unused)))
{
	rtc_setup();
	for (;;) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);	// Block execution until notified
		gpio_toggle(GPIOC, GPIO13);	// Toggle LED
	}
}

int
main(void)
{
	Calendar calendar;
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
	
	rcc_periph_clock_enable(RCC_GPIOB);	// I2C
	rcc_periph_clock_enable(RCC_GPIOC);	// LED
	rcc_periph_clock_enable(RCC_I2C1);	// I2C
	
	gpio_set_mode(GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		GPIO6 | GPIO7);				// I2C
	gpio_set(GPIOB, GPIO6 | GPIO7);	// rising high level leads the I2C to the idle state.

	gpio_set_mode(GPIOC,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13);					// LED	
	gpio_clear(GPIOC, GPIO13);		// Turn LED off.

	gpio_primary_remap(0, 0);		// AFIO_MAPR_I2C1_REMAP=0, PB6+PB7

	initCalendar(&calendar);

	xTaskCreate(thread_clock, "clock", 64, NULL, configMAX_PRIORITIES - 1, NULL);
	xTaskCreate(thread_LED, "LED", 64, NULL, configMAX_PRIORITIES - 1, &LEDtaskHdlr);
	vTaskStartScheduler();
	
	for (volatile uint8_t i = 0; ; ++i) {  }

	return 0;
}