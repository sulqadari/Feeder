#ifndef DELAY_HAL_H
#define DELAY_HAL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stm32f1xx.h"

typedef struct {
	uint32_t delay;
	uint32_t timestamp;
} Delay_TypeDef;

// #define SYSTICK_DELAY_ENABLE

// SysTick Delay (only ms)
#ifdef SYSTICK_DELAY_ENABLE
	void SysTick_Init();
	uint32_t SysTick_millis();
	void SysTick_delay_ms(uint32_t delay);
#endif

// DWT Delay (ms, us)
void DWT_Init(void);
uint32_t DWT_tick(void);
uint32_t DWT_millis(void);
uint32_t DWT_micros(void);
void DWT_delay_ms(uint32_t delay);
void DWT_delay_us(uint32_t delay);
// DWT Non Blocking Delay (ms, us)
uint8_t DWT_nb_timeout(Delay_TypeDef *dt);
void DWT_nb_delay_ms(Delay_TypeDef *dt, uint32_t delay);
void DWT_nb_delay_us(Delay_TypeDef *dt, uint32_t delay);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DELAY_HAL_H */