add_library(${LIB_NOKIA5110}
	lcd_main.c
	pcd8574.c
)

target_include_directories(${LIB_NOKIA5110}
	PUBLIC ${cmsis_inc_device}
	PUBLIC ${cmsis_inc_core}
	PUBLIC ${PROJECT_SOURCE_DIR}/hal
	PUBLIC ${PROJECT_SOURCE_DIR}/libs/uGUI
)

target_link_libraries(${LIB_NOKIA5110}
	${LIB_HAL}
)