#include "hal_rtc.h"

/*
    RM0008, 18.1
    To enable access to the Backup registers and the RTC, proceed as follows:
    1. RCC->APB1ENR  |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN; // enable power and backup interfaces
    2. PWR->CR |= PWR_CR_DBP; // enable access to RTC and backup registers (RM0008, 5.3.6);
*/

void
RTC_Init(void)
{
    rcc_periph_clock_enable(RCC_PWR);   // Enable Power domain
    rcc_periph_clock_enable(RCC_BKP);   // Backup interface clock enabled
    PWR->CR |= PWR_CR_DBP;              // Access to RTC and Backup registers enabled

    RCC->BDCR |= RCC_BDCR_LSEON;        // Enable LSE
    while (!(RCC->BDCR & RCC_BDCR_LSERDY)) { ; }    // Wait until the LSE clock stabilize

    RCC->BDCR |= RCC_BDCR_RTCSEL_0;     // Set the LSE as the source clock for RTC

    RTC->CRL |= RTC_CRL_CNF;            // Enter the RTC Configuration mode 
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0) { ; }   // Wait until the last write operation is done.

    RTC->CRH |= RTC_CRH_SECIE;          // Second interrupt is enabled.
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0) { ; }   // Wait until the last write operation is done.

    RTC->PRLL = 0x7FFF;                 // Get the signal period of 1 second
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0) { ; }   // Wait until the last write operation is done.

    RTC->CRL &= ~RTC_CRL_CNF;           // Exit from the RTC Configuration mode
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0) { ; }   // Wait until the last write operation is done.

    NVIC_EnableIRQ(RTC_IRQn);
}

void
RTC_IRQHandler(void)
{
    RTC->CRL &= ~RTC_CRL_SECF;
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0) { ; }   // Wait until the last write operation is done.
}