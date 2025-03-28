#include "hal_rtc.h"

/*
    RM0008, 18.1
    To enable access to the Backup registers and the RTC, proceed as follows:
    1. RCC->APB1ENR  |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN; // enable power and backup interfaces
    2. PWR->CR |= PWR_CR_DBP; // enable access to RTC and backup registers (RM0008, 5.3.6);
*/