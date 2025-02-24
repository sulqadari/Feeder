#include "ugui_main.h"

static uint8_t pixmap[LCD_COLUMNS_TOTAL][LCD_ROWS_TOTAL];



static void
draw_pixel(UG_S16 x, UG_S16 y, UG_COLOR c)
{
	if ( x < 0 || x >= LCD_COLUMNS_TOTAL || y < 0 || y >= LCD_ROWS_TOTAL )
		return;
	
	uint16_t mask = y;						// let's pretend y = 46; store it in mask.
	y /= 8; // index within rows.			// Now y = 5, i.e. the row index (from 0 to 5)
	uint8_t* target_byte = &pixmap[x][y];	// get byte
	uint8_t shift = mask % 8; 				// 46 % 8 = 6, i.e the 6th bit in 5th byte, which is matches with index 46

	if (c = C_BLACK) {
		*target_byte |=  (1 << shift);
	} else {
		*target_byte &= ~(1 << shift);
	}
}

void
ugui_init(UG_GUI* gui)
{
	UG_Init(gui, draw_pixel, LCD_WIDTH, LCD_HEIGHT);
	UG_SetBackcolor(C_WHITE);
	UG_SetForecolor(C_BLACK);
}

void
ugui_update(void)
{
	NSS_LOW;
	while (SPI1_IsBusy());
	DATA_MODE;

	for (uint16_t y = 0; y < LCD_ROWS_TOTAL; ++y) {
		for (uint16_t x = 0; x < LCD_COLUMNS_TOTAL; ++x) {
			SPI1_Send(pixmap[x][y]);
		}
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