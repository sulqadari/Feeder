add_library(${LIB_HAL}
	hal_delay.c
	hal_gpio.c
	hal_i2c.c
	hal_rcc.c
	hal_spi.c
	hal_usart.c
)

target_include_directories(${LIB_HAL}
	PUBLIC ${cmsis_inc_device}
	PUBLIC ${cmsis_inc_core}
)