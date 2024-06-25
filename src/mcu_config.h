#ifndef FREEDER_MCU_CONFIG_H
#define FREEDER_MCU_CONFIG_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

extern QueueHandle_t queue_TX1, queue_RX1, queue_TX2, queue_RX2;

void feeder_init(void);

#endif // !FREEDER_MCU_CONFIG_H