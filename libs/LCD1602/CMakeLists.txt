add_library(${LIB_LCD1602}
	lcd1602_i2c.c
	${CMAKE_CURRENT_SOURCE_DIR}/../PCF8574T/PCF8574T.c
)

target_link_libraries(${LIB_LCD1602}
	${LIB_HAL}
)
target_include_directories(${LIB_LCD1602}
	PUBLIC ${cmsis_inc_device}
	PUBLIC ${cmsis_inc_core}
	PUBLIC ${PROJECT_SOURCE_DIR}/hal
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../PCF8574T
)