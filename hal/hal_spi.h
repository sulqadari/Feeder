/*
 * spi.h
 *
 * Created on: 9.05.2019
 *     Author: Admin
 */

#ifndef SPI_HAL_H
#define SPI_HAL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

#define SPI_PORT	GPIOA_BASE

// PORT A

#define DC_PIN		GPIO2
#define SE_PIN		GPIO3

#define SCK_PIN		GPIO5
#define MOSI_PIN	GPIO7
#define RST_PIN		GPIO12

void SPI1_Init(void);
void SPI1_chipEnable(void);
void SPI1_chipDisable(void);
void SPI1_Send(uint8_t data);
uint8_t SPI1_IsBusy(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SPI_HAL_H */
