#ifndef LCD_MAIN_H
#define LCD_MAIN_H

#include <stdint.h>
#include <string.h>
#include "pcd8574.h"
#include "ugui.h"

void lcd_init(UG_GUI* gui);
void lcd_set_font(const UG_FONT* font);
void lcd_update(void);
void lcd_clear_pixmap(void);
void lcd_circle(UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c);
void lcd_print_string(UG_S16 x, UG_S16 y, char* str);

#endif /* LCD_MAIN_H */