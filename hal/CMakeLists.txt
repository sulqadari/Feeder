add_library(${LIB_HAL}
	hal_rcc.c
	hal_gpio.c
	hal_i2c.c
	hal_delay.c
)

target_include_directories(${LIB_HAL}
	PUBLIC ${cmsis_inc_device}
	PUBLIC ${cmsis_inc_core}
)