#ifndef EXTI_HAL_H
#define EXTI_HAL_H

#include "hal_rcc.h"
#include "hal_gpio.h"

extern uint8_t left_btn;

void EXTI_Init(void);

#endif /* EXTI_HAL_H */