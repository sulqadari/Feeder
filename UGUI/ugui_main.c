#include "ugui_main.h"

static uint8_t pixmap[LCD_BUFFER_SIZE];

static uint8_t*
get_pixel(UG_S16 x, UG_S16 y, uint32_t* bitNo)
{
	*bitNo = 7 - y % 8;

	uint32_t inv_y = LCD_HEIGHT - y;
	uint32_t pageNo = inv_y / 8;
	uint32_t colNo = x % LCD_WIDTH;
	return &pixmap[pageNo * LCD_WIDTH + colNo];
}

static void
draw_pixel(UG_S16 x, UG_S16 y, UG_COLOR c)
{
	if ( x < 0 || x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT )
		return;
	
	uint32_t bitNo;
	uint8_t* byte = get_pixel(x, y, &bitNo);
	uint8_t mask = 1 << bitNo;

	if (c == C_BLACK) {
		*byte |= mask;
	} else {
		*byte &= ~mask;
	}
}

static void
draw_point(UG_S16 x, UG_S16 y, UG_COLOR c)
{
	draw_pixel(x, y, c);
}

void
ugui_init(UG_GUI* gui)
{
	UG_Init(gui, draw_point, LCD_WIDTH, LCD_HEIGHT);
	UG_SetBackcolor(C_WHITE);
	UG_SetForecolor(C_BLACK);
}

void
ugui_update(void)
{
	NSS_LOW;
	while (SPI1_IsBusy());
	DATA_MODE;

	for (uint16_t i = 0; i < LCD_BUFFER_SIZE; ++i) {
		SPI1_Send(pixmap[i]);
	}

	while (SPI1_IsBusy());
	NSS_HIGH;

	n5110_set_cursor(0, 0);
}

void
ugui_draw_frame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c)
{
	UG_DrawFrame(x1, y1, x2, y2, c);
	ugui_update();
}

void
ugui_draw_pixel(UG_S16 x0, UG_S16 y0, UG_COLOR c)
{
	UG_DrawPixel(x0, y0, c);
	ugui_update();
}