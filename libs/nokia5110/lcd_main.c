#include "lcd_main.h"

static uint8_t pixmap[LCD_COLUMNS_TOTAL][LCD_ROWS_TOTAL];

static void
draw_pixel(UG_S16 x, UG_S16 y, UG_COLOR c)
{
	if ( x < 0 || x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT )
		return;
	
	uint16_t mask = y;						// let's pretend y = 46; store it in mask.
	y /= 8; // index within rows.			// Now y = 5, i.e. the row index (from 0 to 5)
	uint8_t* target_byte = &pixmap[x][y];	// get byte
	uint8_t shift = mask % 8; 				// 46 % 8 = 6, i.e the 6th bit in 5th byte, which is matches with index 46

	if (c == C_BLACK) {
		*target_byte |=  (1 << shift);
	} else {
		*target_byte &= ~(1 << shift);
	}
}

void
lcd_init(UG_GUI* gui)
{
	n5110_init();
	UG_Init(gui, draw_pixel, LCD_WIDTH, LCD_HEIGHT);
	UG_FontSelect(&FONT_5X12);
	UG_SetBackcolor(C_WHITE);
	UG_SetForecolor(C_BLACK);
	n5110_set_cursor(0, 0);
}

void
lcd_set_font(const UG_FONT* font)
{
	UG_FontSelect(font);
}

static void
update_pixmap(void)
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
lcd_clear_pixmap(void)
{
	memset(pixmap, 0x00, LCD_BUFFER_SIZE);
}

void
lcd_circle(UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c)
{
	UG_DrawCircle(x0, y0, r, c);
	UG_FillCircle(x0, y0, r, c);
	update_pixmap();
}

void
lcd_print_string(UG_S16 x, UG_S16 y, char* str)
{
	UG_PutString(x, y, str);
	update_pixmap();
}