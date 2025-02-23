add_library(${LIB_NOKIA5110}
	nokia5110.c
)

target_include_directories(${LIB_NOKIA5110}
	PUBLIC ${cmsis_inc_device}
	PUBLIC ${cmsis_inc_core}
	PUBLIC ${PROJECT_SOURCE_DIR}/hal
	PUBLIC ${PROJECT_SOURCE_DIR}/libs/graphics
	PUBLIC ${PROJECT_SOURCE_DIR}/UGUI
)

target_link_libraries(${LIB_NOKIA5110}
	${LIB_HAL}
	${LIB_UGUI}
)