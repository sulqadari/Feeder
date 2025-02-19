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
 * Legend:
 * The following pins should connect to SPI1 interface of the STM32F103 with the dedicated pins on the slave device (n5110)
 * CE_PIN(GPIO_A4)   ---> SE (chip select)
 * SCK_PIN(GPIO_A5)  ---> CLK
 * MOSI_PIN(GPIO_A7) ---> DIN
 * 
 * RST_PIN(GPIO_B4)  ---> RST
 * PB5_PIN(GPIO_B5)  ---> BL (Backlight)
 * DC_PIN(GPIO_B6)   ---> DATA/COMMAND
 */
void
SPI1_Init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_SPI1);

	gpio_set_mode(
		GPIOB_BASE,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		RST_PIN  | DC_PIN);

	gpio_set_mode(
		GPIOB_BASE,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_OPENDRAIN,
		BL_PIN);
	
	gpio_set_mode(
		GPIOA_BASE,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		CE_PIN);

	gpio_set_mode(
		GPIOA_BASE,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		SCK_PIN | MOSI_PIN);
	
	SPI1->CR1 = SPI_CR1_MSTR	// Configuring the SPI as the master
				| SPI_CR1_BR_2 	// Choose divisor 72MHz / 32 = 2.25 MHz speed
				| SPI_CR1_SSM	// Software slave management enabled
				| SPI_CR1_SSI	// Force HIGH on NSS pin (Has effect only when the SPI_CR1_SSM=1)
				| SPI_CR1_SPE;	// Enable the SPI
}

void
SPI1_chipEnable(void)
{
	GPIOA->BRR |= GPIO_BRR_BR4;	// 1: Reset the corresponding ODRx bit
}

void
SPI1_chipDisable(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS4;	// 1: Set the corresponding ODRx bit
}

void
SPI1_Send(uint16_t data)
{
	while ((SPI1->SR & SPI_SR_TXE) == RESET);	// Wait until the transmit buffer becomes empty
	SPI1->DR = data;
}

uint8_t
SPI1_IsBusy(void)
{
	return SPI1->SR & SPI_SR_BSY;
}