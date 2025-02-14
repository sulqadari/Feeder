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

void SPI1_Init() {

	// SPI1
	SPI1->CR1 = SPI_CR1_MSTR					// 1: Master configuration
				| SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0 // 110: fPCLK/256
				| SPI_CR1_SSI					// SSI: Internal slave select
				| SPI_CR1_SSM					// 1: Software slave management enabled
				| SPI_CR1_CPHA;

	#ifdef SPI1_16_BIT_FORMAT
	SPI1->CR1 |= SPI_CR1_DFF;					// 1: 16-bit data frame format is selected for transmission/reception
	#endif

	SPI1->CR1 |= SPI_CR1_SPE;					// 1: Peripheral enabled
}

void SPI1_EnableSlave() {

	GPIOA->BRR |= GPIO_BRR_BR4;					// 1: Reset the corresponding ODRx bit
}

void SPI1_DisableSlave() {

	GPIOA->BSRR |= GPIO_BSRR_BS4;				// 1: Set the corresponding ODRx bit
}

uint16_t SPI1_Write(uint16_t data) {

	while (!(SPI1->SR & SPI_SR_TXE));			// 0: Tx buffer not empty
	SPI1->DR = data;

	// while (!(SPI1->SR & SPI_SR_TXE));			// 0: Tx buffer not empty
	// while (!(SPI1->SR & SPI_SR_RXNE));			// 0: Rx buffer empty
	// while (SPI1->SR & SPI_SR_BSY);				// 1: SPI (or I2S) is busy in communication or Tx buffer is not empty

	return (uint16_t)SPI1->DR;
}

uint8_t
SPI1_IsBusy(void)
{
	return SPI1->SR & SPI_SR_BSY;
}