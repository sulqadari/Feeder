#include "hal_exti.h"

uint8_t btnLeft = 0;

void
EXTI_Init(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_AFIO);

    gpio_set_mode(
        GPIOA_BASE,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_PULL_UPDOWN,
        GPIO0
    );

    GPIOA->ODR |= 0x01;             // pull-up resistor
    EXTI->RTSR |= EXTI_RTSR_RT0;    // Rising trigger enabled for PA0 (RM0008, 10.3.3).

    EXTI->PR = EXTI_PR_PR0;         // Clear interrupt flag (just in case?). See RM0008, 10.3.6
    EXTI->IMR |= EXTI_IMR_IM0;      // Interrupt mask on line '0'
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void
EXTI0_IRQHandler(void)
{
    EXTI->PR = EXTI_PR_PR0;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    btnLeft = 1;
    /* TODO: button press handler. */
}