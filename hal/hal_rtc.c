#include "hal_rtc.h"

uint32_t test_counter;

void
RTC_Init(void)
{
	test_counter = 0;
    RCC_Periph_clock_en(RCC_PWR);   // Enable access Power domain.
    RCC_Periph_clock_en(RCC_BKP);   // Enable access Backup interface.
    PWR->CR |= PWR_CR_DBP;              // Enable access to backup domain.


	RCC->BDCR |= RCC_BDCR_LSEON;        // Enable the LSE clock.
	while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0) { ; }// Wait until the LSE clock stabilize.

	RCC->BDCR |= RCC_BDCR_RTCSEL_0;     // Select LSE as the RTC clock source.
	RCC->BDCR |= RCC_BDCR_RTCEN;        // Enable the RTC clock.
	
	while ((RTC->CRL & RTC_CRL_RSF) == 0) { ; }     // Wait until the RTC registers become synchronized.
	while ((RTC->CRL & RTC_CRL_RTOFF) == 0) { ; }   // Wait until the last write operation is done.
	
	RTC->CRH |= RTC_CRH_SECIE;          // Enable the "second" global interrupt.
	while ((RTC->CRL & RTC_CRL_RTOFF) == 0) { ; }   // Wait until the last write operation is done.

	RTC->PRLL = 0x7FFF;                 // Get the signal period of 1 second
	while ((RTC->CRL & RTC_CRL_RTOFF) == 0) { ; }   // Wait until the last write operation is done.

    NVIC_EnableIRQ(RTC_IRQn);
}

void
RTC_IRQHandler(void)
{
    RTC->CRL &= ~RTC_CRL_SECF;
    while ((RTC->CRL & RTC_CRL_RTOFF) == 0) { ; }   // Wait until the last write operation is done.
	test_counter = RTC->CNTL;
    test_counter |= (uint32_t)(RTC->CNTH << 16);
}