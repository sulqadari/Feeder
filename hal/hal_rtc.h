#ifndef RTC_HAL_H
#define RTC_HAL_H

#include "hal_gpio.h"
#include "hal_rcc.h"

// Тип дата+время в количестве секунд с начала эпохи
typedef uint32_t datetime_t;
extern datetime_t dateTimeCounter;

void RTC_Init(void);
void RTC_SetTime(datetime_t update);

#endif /* RTC_HAL_H */