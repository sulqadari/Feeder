add_library(${LIB_HAL}
	hal_wdt.c
	hal_gpio.c
	hal_rcc.c
	hal_spi.c
	hal_exti.c
	hal_rtc.c
)

target_include_directories(${LIB_HAL}
	PUBLIC ${cmsis_inc_device}
	PUBLIC ${cmsis_inc_core}
)