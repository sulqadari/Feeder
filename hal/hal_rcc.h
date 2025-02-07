#ifndef RCC_HAL_H
#define RCC_HAL_H

#include <stdint.h>
#include "stm32f1xx.h"
#include "hal_gpio.h"

#define _RCC_REG(i)			MMIO32(RCC_BASE + ((i) >> 5))
#define _RCC_BIT(i)			(1 << ((i) & 0x1f))
#define _REG_BIT(base, bit)	(((base) << 5) + (bit))

enum rcc_periph_clken {

	/* AHB peripherals */
	RCC_DMA1	= _REG_BIT(0x14, 0),/*VNC*/
	RCC_DMA2	= _REG_BIT(0x14, 1),/*VNC*/
	RCC_SRAM	= _REG_BIT(0x14, 2),/*VNC*/
	RCC_FLTF	= _REG_BIT(0x14, 4),/*VNC*/
	RCC_CRC		= _REG_BIT(0x14, 6),/*VNC*/
	RCC_FSMC	= _REG_BIT(0x14, 8),/*VN-*/
	RCC_SDIO	= _REG_BIT(0x14, 10),/*-N-*/
	RCC_OTGFS	= _REG_BIT(0x14, 12),/*--C*/
	RCC_ETHMAC	= _REG_BIT(0x14, 14),/*--C*/
	RCC_ETHMACTX	= _REG_BIT(0x14, 15),/*--C*/
	RCC_ETHMACRX	= _REG_BIT(0x14, 16),/*--C*/

	/* APB2 peripherals */
	RCC_AFIO	= _REG_BIT(0x18, 0),/*VNC*/
	RCC_GPIOA	= _REG_BIT(0x18, 2),/*VNC*/
	RCC_GPIOB	= _REG_BIT(0x18, 3),/*VNC*/
	RCC_GPIOC	= _REG_BIT(0x18, 4),/*VNC*/
	RCC_GPIOD	= _REG_BIT(0x18, 5),/*VNC*/
	RCC_GPIOE	= _REG_BIT(0x18, 6),/*VNC*/
	RCC_GPIOF	= _REG_BIT(0x18, 7),/*VN-*/
	RCC_GPIOG	= _REG_BIT(0x18, 8),/*VN-*/
	RCC_ADC1	= _REG_BIT(0x18, 9),/*VNC*/
	RCC_ADC2	= _REG_BIT(0x18, 10),/*-NC*/
	RCC_TIM1	= _REG_BIT(0x18, 11),/*VNC*/
	RCC_SPI1	= _REG_BIT(0x18, 12),/*VNC*/
	RCC_TIM8	= _REG_BIT(0x18, 13),/*-N-*/
	RCC_USART1	= _REG_BIT(0x18, 14),/*VNC*/
	RCC_ADC3	= _REG_BIT(0x18, 15),/*-N-*/
	RCC_TIM15	= _REG_BIT(0x18, 16),/*V--*/
	RCC_TIM16	= _REG_BIT(0x18, 17),/*V--*/
	RCC_TIM17	= _REG_BIT(0x18, 18),/*V--*/
	RCC_TIM9	= _REG_BIT(0x18, 19),/*-N-*/
	RCC_TIM10	= _REG_BIT(0x18, 20),/*-N-*/
	RCC_TIM11	= _REG_BIT(0x18, 21),/*-N-*/

	/* APB1 peripherals */
	RCC_TIM2	= _REG_BIT(0x1C, 0),/*VNC*/
	RCC_TIM3	= _REG_BIT(0x1C, 1),/*VNC*/
	RCC_TIM4	= _REG_BIT(0x1C, 2),/*VNC*/
	RCC_TIM5	= _REG_BIT(0x1C, 3),/*VNC*/
	RCC_TIM6	= _REG_BIT(0x1C, 4),/*VNC*/
	RCC_TIM7	= _REG_BIT(0x1C, 5),/*VNC*/
	RCC_TIM12	= _REG_BIT(0x1C, 6),/*VN-*/
	RCC_TIM13	= _REG_BIT(0x1C, 7),/*VN-*/
	RCC_TIM14	= _REG_BIT(0x1C, 8),/*VN-*/
	RCC_WWDG	= _REG_BIT(0x1C, 11),/*VNC*/
	RCC_SPI2	= _REG_BIT(0x1C, 14),/*VNC*/
	RCC_SPI3	= _REG_BIT(0x1C, 15),/*VNC*/
	RCC_USART2	= _REG_BIT(0x1C, 17),/*VNC*/
	RCC_USART3	= _REG_BIT(0x1C, 18),/*VNC*/
	RCC_UART4	= _REG_BIT(0x1C, 19),/*VNC*/
	RCC_UART5	= _REG_BIT(0x1C, 20),/*VNC*/
	RCC_I2C1	= _REG_BIT(0x1C, 21),/*VNC*/
	RCC_I2C2	= _REG_BIT(0x1C, 22),/*VNC*/
	RCC_USB		= _REG_BIT(0x1C, 23),/*-N-*/
	RCC_CAN		= _REG_BIT(0x1C, 25),/*-N-*/
	RCC_CAN1	= _REG_BIT(0x1C, 25),/*--C*/
	RCC_CAN2	= _REG_BIT(0x1C, 26),/*--C*/
	RCC_BKP		= _REG_BIT(0x1C, 27),/*VNC*/
	RCC_PWR		= _REG_BIT(0x1C, 28),/*VNC*/
	RCC_DAC		= _REG_BIT(0x1C, 29),/*VNC*/
	RCC_CEC		= _REG_BIT(0x1C, 30),/*V--*/
};

int32_t rcc_set_hse72(void);
void rcc_periph_clock_enable(enum rcc_periph_clken clken);

#endif /* RCC_HAL_H */