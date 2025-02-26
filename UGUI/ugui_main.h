#ifndef UGUI_MAIN_H
#define UGUI_MAIN_H

#include "ugui.h"
#include "nokia5110.h"

void ugui_init(UG_GUI* gui);
void ugui_update(void);
void ugui_frame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
void ugui_pixel(UG_S16 x0, UG_S16 y0, UG_COLOR c);
void ugui_circle(UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c);
void ugui_clear_pixmap(void);
void ugui_line(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);

void ugui_print_string(UG_S16 x, UG_S16 y, char* str);
void ugui_set_font(const UG_FONT* font);

#endif /* UGUI_MAIN_H */