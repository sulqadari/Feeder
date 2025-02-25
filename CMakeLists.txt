cmake_minimum_required(VERSION 3.25)
project(Feeder VERSION 1.0)


enable_language(ASM C)
set(CMAKE_C_COMPILER	"arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER	"arm-none-eabi-gcc")
set(CMAKE_LINKER		"arm-none-eabi-ld")
set(OBJCOPY				"arm-none-eabi-objcopy")
set(OBJDUMP				"arm-none-eabi-objdump")
set(SIZE				"arm-none-eabi-size")

set(ENTRY_POINT		"FeedMe")
set(LIB_HAL			"_hal")
set(LIB_LCD1602		"_lcd1602")
set(LIB_NOKIA5110	"_nokia5110")
set(LIB_UGUI		"_uGUI")

set(cmsis_path			${PROJECT_SOURCE_DIR}/hal/CMSIS)
set(cmsis_inc_device	${cmsis_path}/Device/ST/STM32F1xx/Include)
set(cmsis_inc_core		${cmsis_path}/Include)
set(cmsis_src_templates ${cmsis_path}/Device/ST/STM32F1xx/Source/Templates)

add_compile_definitions(
	STM32F103xB STM32F1
)

add_compile_options(
	-Wall
	-O0
	-std=c99
	-g
	-mcpu=cortex-m3
	-fdata-sections -ffunction-sections
)

add_link_options(
	-mcpu=cortex-m3
	-mthumb
	"LINKER:--gc-sections"	# used in conjunction with '-fdata-sections -ffunction-sections'
	"LINKER:-T,${CMAKE_SOURCE_DIR}/src/STM32F103XB_FLASH.ld"
	"LINKER:-Map=${ENTRY_POINT}.map"
)


add_subdirectory(src)
add_subdirectory(hal)
add_subdirectory(libs/nokia5110)
add_subdirectory(UGUI)