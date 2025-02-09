#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
 
typedef enum {
	I2C_STANDARD,
	I2C_FAST
} I2C_Mode_Type;

typedef enum {
	I2C_TRANSMITTER,
	I2C_RECEIVER
} I2C_Direction_Type;

typedef enum {
	I2C_ACK,
	I2C_NACK
} I2C_Acknowledge_Type;

typedef enum {
	I2C_ERROR = 0x0,
	I2C_OK
} I2C_Status_Type;

void I2C1_Init(I2C_Mode_Type mode);
void I2C1_Start(void);
void I2C1_Stop(void);
I2C_Status_Type I2C1_SendAddress(uint8_t address, I2C_Direction_Type direction);
void I2C1_SendData(uint8_t data);
uint8_t I2C1_ReceiveData(I2C_Acknowledge_Type acknowledge);

#endif /* HAL_I2C_H */