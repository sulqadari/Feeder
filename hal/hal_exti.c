#include "hal_exti.h"
#include <string.h>

uint8_t which_one = 0xff;

void
EXTI_Init(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_AFIO);

    gpio_set_mode(
        GPIOA_BASE,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT,
        GPIO0 | GPIO1 | GPIO2 | GPIO3
    );
    
	EXTI->RTSR	|= EXTI_RTSR_RT0 | EXTI_RTSR_RT1	// Rising trigger enabled for PA0 (RM0008, 10.3.3).
				|  EXTI_RTSR_RT2 | EXTI_RTSR_RT3;

    EXTI->PR	= EXTI_PR_PR0   | EXTI_PR_PR1		// Enable interrupt request. see 10.2.4
				|  EXTI_PR_PR2  | EXTI_PR_PR3;
    
	EXTI->IMR	|= EXTI_IMR_IM0 | EXTI_IMR_IM1		// Unmask interrupt mask
				|  EXTI_IMR_IM2 | EXTI_IMR_IM3;

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
}

#define EXTI_IDLE			\
volatile int i = 10;		\
do {						\
	i--;					\
	__ASM volatile ("nop");	\
} while (i > 0)				\

void
EXTI0_IRQHandler(void)
{
	EXTI->PR = EXTI_PR_PR0;	// Reset interrupt request on line "0". see 10.2.4
    
    __ASM volatile ("nop"); __ASM volatile ("nop");
    __ASM volatile ("nop"); __ASM volatile ("nop");
	__ASM volatile ("nop"); __ASM volatile ("nop");

	if (which_one == 0xff)
		which_one = 0;

}

void
EXTI1_IRQHandler(void)
{
	EXTI->PR = EXTI_PR_PR1;	// Reset interrupt request on line "1". see 10.2.4
	
    __ASM volatile ("nop"); __ASM volatile ("nop");
    __ASM volatile ("nop"); __ASM volatile ("nop");
	__ASM volatile ("nop"); __ASM volatile ("nop");
	
	if (which_one == 0xff)
		which_one = 1;
}

void
EXTI2_IRQHandler(void)
{
	EXTI->PR = EXTI_PR_PR2;	// Reset interrupt request on line "2". see 10.2.4

    __ASM volatile ("nop"); __ASM volatile ("nop");
    __ASM volatile ("nop"); __ASM volatile ("nop");
	__ASM volatile ("nop"); __ASM volatile ("nop");
	
	if (which_one == 0xff)
		which_one = 2;
}

void
EXTI3_IRQHandler(void)
{
	EXTI->PR = EXTI_PR_PR3;	// Reset interrupt request on line "3". see 10.2.4
	
    __ASM volatile ("nop"); __ASM volatile ("nop");
    __ASM volatile ("nop"); __ASM volatile ("nop");
	__ASM volatile ("nop"); __ASM volatile ("nop");
	
	if (which_one == 0xff)
		which_one = 3;
}