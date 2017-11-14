#ifndef GRAPHICS_NOKIA_H
#define GRAPHICS_NOKIA_H

#include <stm8/stm8s.h>
#include <stm8/timing.h>

/*

PC4 = CE#
PC3 = DC
PD2 = RST

*/

#ifndef NULL
#define NULL ((void*)0)
#endif

uint8_t screen[504];
const uint16_t screen_size = 504;
uint8_t text_x = 0;
uint8_t text_y = 0;
const uint8_t* default_font = NULL;

void InitScreen()
{
	SPI_CR2 |= (1<<1)|(1<<0); //SSM (software slave mgmt) on, SSI on (master mode)
	SPI_CR1 |= (1<<6) | (1<<3) | (1<<2);  //enable spi, Fmaster/4, master mode
	
	PC_DDR |= (1<<4) | (1<<3); //enable output on CS# and DC lines
	PC_CR1 |= (1<<4) | (1<<3); //enable push/pull on CS# and DC lines
	PC_ODR |= (1<<4); //make CS# high to disable all chips
	
	PD_DDR |= (1<<2); //enable output on RST
	PD_ODR &= ~(1<<2); //set RST LOW
	PD_CR1 |= (1<<2); //enable push/pull on RST
	
	pause_ds(1);
	
	PD_ODR |= (1<<2); //set RST HIGH
	
	PC_ODR &= ~(1<<3); //DC LOW
	PC_ODR &= ~(1<<4); //assert CS# low
	SPI_transfer(0x23); //PD=0, Vertical, Extended
	SPI_transfer(0x13); //set bias
	SPI_transfer(0xC7); //set VOp (BE for newer one, C7 for the older one)
	SPI_transfer(0x22); //Power Down disabled, Vertical mode, basic instruction set
	SPI_transfer(0x0C); //NORMAL display mode
	SPI_transfer(0x40); //set Y addr = 0
	SPI_transfer(0x80); //set X addr = 0
	PC_ODR |= (1<<4); //release CS# (make it high)
}

void UpdateScreen(uint8_t* buf, uint16_t size)
{
	PC_ODR &= ~(1<<3); //DC LOW
	PC_ODR &= ~(1<<4); //assert CS# low
	SPI_transfer(0x22); //Power Down disabled, Vertical mode, basic instruction set
	SPI_transfer(0x40); //set Y addr = 0
	SPI_transfer(0x80); //set X addr = 0
	PC_ODR |= (1<<3); //DC HIGH
	SPI_cmd(buf, size, NULL, 0);
	PC_ODR |= (1<<4); //release CS# (make it high)
}

void ClearScreen(uint8_t* buf, uint16_t size)
{
	uint16_t i = 0;
	for (; i < size; ++i) {
		buf[i] = 0;
	}
}

//height in bytes
void PlotPoint(uint8_t* buf, uint8_t height, uint8_t x, uint8_t y)
{
	buf[x * height + y / 8] = 1 << (y % 8);
}

//height in bytes
void DrawChar(uint8_t* buf, uint8_t height, const uint8_t* font, uint8_t char_width, char c, uint8_t x, uint8_t y)
{
	int i;
	for (i = 0; i < char_width; ++i) {
		buf[(i + x * char_width) * height + y] |= font[c * char_width + i];
	}
}

void ResetCursor()
{
	text_x = 0;
	text_y = 0;
}

#ifdef GRAPHICS_NOKIA_WANT_PUTCHAR
int putchar(int c)
{
	if (c == '\n') {
		text_x = 0;
		++text_y;
	}
	else {
		DrawChar(screen, 6, default_font, 6, c, text_x, text_y);
		++text_x;
		text_y += text_x / 14;
		text_x = text_x % 14;
	}
	return c;
}
#endif //GRAPHICS_NOKIA_WANT_PUTCHAR

#endif //GRAPHICS_NOKIA_H