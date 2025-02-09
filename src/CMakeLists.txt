add_executable(${ENTRY_POINT}.elf
	main.c
	startup_stm32f103xb.s
	${cmsis_src_templates}/system_stm32f1xx.c
)

target_include_directories(${ENTRY_POINT}.elf
	PUBLIC
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_SOURCE_DIR}/hal
		${PROJECT_SOURCE_DIR}/libs/LCD1602
		${cmsis_inc_device}
		${cmsis_inc_core}
)

target_link_libraries(${ENTRY_POINT}.elf
	${LIB_HAL}
	${LIB_LCD1602}
)

add_custom_command(TARGET ${ENTRY_POINT}.elf
	POST_BUILD
	COMMAND ${OBJCOPY} -O binary ${ENTRY_POINT}.elf ${PROJECT_SOURCE_DIR}/${ENTRY_POINT}.bin
	BYPRODUCTS ${ENTRY_POINT}.bin
)

add_custom_command(TARGET ${ENTRY_POINT}.elf
	POST_BUILD
	COMMAND ${OBJCOPY} -O ihex ${ENTRY_POINT}.elf ${ENTRY_POINT}.hex
	BYPRODUCTS ${ENTRY_POINT}.hex
)

add_custom_command(TARGET ${ENTRY_POINT}.elf
	POST_BUILD
	COMMAND ${SIZE} ${ENTRY_POINT}.elf
)
