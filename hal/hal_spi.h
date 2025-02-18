/*
 * spi.h
 *
 * Created on: 9.05.2019
 *     Author: Admin
 */

#ifndef SPI_H_
#define SPI_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

#define SPI_PORT	GPIOA_BASE
#define RST_PIN		GPIO2
#define DC_PIN		GPIO3
#define CE_PIN		GPIO4
#define SCK_PIN		GPIO5
#define MOSI_PIN	GPIO7

#define LED_PIN		GPIO13

void SPI1_Init(void);
void SPI1_chipEnable(void);
void SPI1_chipDisable(void);
void SPI1_Send(uint16_t data);
uint8_t SPI1_IsBusy(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SPI_H_ */
