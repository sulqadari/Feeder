#include "hal_rcc.h"

/** See RM0008, clause 7.3 'RCC registers'. */
int32_t
rcc_set_hse72(void)
{
	RCC->CR   |= RCC_CR_HSEON;		// Enable 8 MHz oscillator
	while (1) {						// Loop until HSE clock source is ready.
		if (RCC->CR & RCC_CR_HSERDY)
			break;
	}

	RCC->CFGR |= RCC_CFGR_PLLMULL9;	// PLL multiplication factor 9: 8 * 9 = 72 MHz
	RCC->CFGR |= RCC_CFGR_PLLSRC;	// Seelct PLL as clock source.

	RCC->CR   |= RCC_CR_PLLON;		// Enable PLL
	while (1) {						// Loop until PLL clock source is ready.
		if (RCC->CR & RCC_CR_PLLRDY)
			break;
	}

	FLASH->ACR |= FLASH_ACR_LATENCY_2;	// RM0008, clause 3.3.3 'FLASH_ACR'
	RCC->CFGR  |= RCC_CFGR_HPRE_DIV1;	// AHB prescaller equals 1 (72 MHz)
	RCC->CFGR  |= RCC_CFGR_PPRE1_DIV2;	// APB1 prescaller equals 2 (36 MHz)
	RCC->CFGR  |= RCC_CFGR_PPRE2_DIV1;	// APB2 prescaller equals 1 (72 MHz)

	RCC->CFGR  |= RCC_CFGR_SW_PLL;		// Select PLL as SYSCLK source.

	while (1) {							// Loop until PLL is set as SYSCLK source
		if (RCC->CFGR & RCC_CFGR_SWS_PLL)
			break;
	}

	RCC->CR &= ~RCC_CR_HSION;			// Turn HSI oscillator off
	
	return 0;
}

void
rcc_periph_clock_enable(enum rcc_periph_clken clken)
{
	_RCC_REG(clken) |= _RCC_BIT(clken);
}