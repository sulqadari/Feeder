add_library(${LIB_HAL}
	hal_rcc.c
	hal_gpio.c
)

target_include_directories(${LIB_HAL}
	PUBLIC ${cmsis_inc_device}
	PUBLIC ${cmsis_inc_core}
)