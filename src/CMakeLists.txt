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
	utils/mcuio.c
	utils/usbcdc.c
	utils/miniprintf.c
	utils/getline.c

	# USB stack
	${hal_path}/lib/usb/usb.c
	${hal_path}/lib/usb/usb_control.c
	${hal_path}/lib/usb/usb_standard.c
	${hal_src_stm32}/st_usbfs_v1.c
	${hal_src_stm32}/common/st_usbfs_core.c
)

target_include_directories(${ENTRY_POINT}.elf
	PUBLIC
		${PROJECT_SOURCE_DIR}/includes
		${PROJECT_SOURCE_DIR}/includes/utils
		${rtos_sources}/include
		${rtos_portable}
		${hal_path}/include
		${hal_path}/include/libopencm3/stm32/common
		${hal_path}/include/libopencm3/stm32/f1
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
