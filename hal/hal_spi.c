/*
 * SPI1
 * NSS : PA4 (General purpose output push-pull)
 * SCK : PA5 (Alternate function push-pull)
 * MISO: PA6 (Input floating / Input pull-up)
 * MOSI: PA7 (Alternate function push-pull)
 */
#include "hal_spi.h"
#include "hal_rcc.h"
#include "hal_gpio.h"

/**
 * This configuration is intended for one-directional communication with n5110 display.
 */
void
SPI2_Init(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_SPI2);
	
	gpio_set_mode(
		GPIOB_BASE,
		GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		DC_PIN | SE_PIN | RST_PIN);

	gpio_set_mode(
		GPIOB_BASE,
		GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		SCK_PIN | MOSI_PIN);
	
	SPI2->CR1 = SPI_CR1_MSTR	// Configuring the SPI as the master
				| SPI_CR1_BR_2	// Choose divisor 72MHz / 32 = 2.25 MHz speed
				| SPI_CR1_SSM	// Software slave management enabled
				| SPI_CR1_SSI	// Force HIGH on NSS pin (Has effect only when the SPI_CR1_SSM=1)
				| SPI_CR1_SPE;	// Enable the SPI 
}

void
SPI2_chipEnable(void)
{
	GPIOB->BRR |= GPIO_BRR_BR4;	// 1: Reset the corresponding ODRx bit
}

void
SPI2_chipDisable(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BS4;	// 1: Set the corresponding ODRx bit
}

void
SPI2_Send(uint8_t data)
{
	while ((SPI2->SR & SPI_SR_TXE) == RESET);	// Wait until the transmit buffer becomes empty
	SPI2->DR = data;
}

uint8_t
SPI2_IsBusy(void)
{
	return SPI2->SR & SPI_SR_BSY;
}