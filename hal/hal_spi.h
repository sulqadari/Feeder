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

// PORT B

#define DC_PIN		GPIO5
#define SE_PIN		GPIO6
#define RST_PIN		GPIO7

#define SCK_PIN		GPIO13
#define MOSI_PIN	GPIO15

void SPI2_Init(void);
void SPI2_chipEnable(void);
void SPI2_chipDisable(void);
void SPI2_Send(uint8_t data);
uint8_t SPI2_IsBusy(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SPI_HAL_H */
