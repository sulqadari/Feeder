add_executable(${ENTRY_POINT}.elf
	main.c
	startup_stm32f103xb.s
	${hal_src_stm32_f1}/gpio.c
	${hal_src_stm32_f1}/rcc.c
	${hal_src_stm32_f1}/rtc.c
	${hal_src_cm3}/nvic.c
	${rtos_sources}/tasks.c
	${rtos_sources}/list.c
	${rtos_sources}/portable/MemMang/heap_4.c
	${rtos_sources}/queue.c
	${rtos_portable}/port.c
	${hal_src_stm32_cmn}/rcc_common_all.c
	${hal_src_stm32_cmn}/gpio_common_all.c
	${hal_src_stm32_cmn}/flash_common_all.c
	${hal_src_stm32_cmn}/pwr_common_v1.c
	
)

target_include_directories(${ENTRY_POINT}.elf
	PUBLIC
		${PROJECT_SOURCE_DIR}/includes
		${rtos_sources}/include
		${rtos_portable}
		${hal_path}/include
)

add_custom_command(TARGET ${ENTRY_POINT}.elf
	POST_BUILD
	COMMAND ${OBJCOPY} -O binary ${ENTRY_POINT}.elf ${PROJECT_SOURCE_DIR}/${ENTRY_POINT}.bin
	BYPRODUCTS SeOS.bin
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
