#ifndef GRAPHICS_NOKIA_H
#define GRAPHICS_NOKIA_H

#include "timing.h"
#include <stdint.h>

/*

PC4 = CE#
PC3 = DC
PD2 = RST

*/

#ifndef NULL
#define NULL ((void*)0)
#endif

extern uint8_t screen[504];
extern const uint16_t screen_size;
extern uint8_t text_x;
extern uint8_t text_y;
extern const uint8_t* default_font;

void InitScreen(void);
void UpdateScreen(uint8_t* buf, uint16_t size);

void ClearScreen(uint8_t* buf, uint16_t size);

//height in bytes
void PlotPoint(uint8_t* buf, uint8_t height, uint8_t x, uint8_t y);

//height in bytes
void DrawChar(uint8_t* buf, uint8_t height, const uint8_t* font, uint8_t char_width, char c, uint8_t x, uint8_t y);

void ResetCursor(void);

#ifdef GRAPHICS_NOKIA_WANT_PUTCHAR
char putchar(char c);
#endif //GRAPHICS_NOKIA_WANT_PUTCHAR

#endif //GRAPHICS_NOKIA_H