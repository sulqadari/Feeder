add_library(${LIB_UGUI}
	ugui.c
	ugui_fonts.c
	miniprintf.c
)

target_include_directories(${LIB_UGUI}
	PUBLIC ${cmsis_inc_device}
	PUBLIC ${cmsis_inc_core}
	PUBLIC ${PROJECT_SOURCE_DIR}/hal
	PUBLIC ${PROJECT_SOURCE_DIR}/libs/graphics
)
