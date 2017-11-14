#define F_CPU 16000000L

#include <stdint.h>
#include <stm8/stm8s.h>
#include <stm8/timing.h>
#include <stm8/SPI.h>
#include <stdio.h>

#define GRAPHICS_NOKIA_WANT_PUTCHAR
#include <stm8/nokia_5110_lcd.h>

#include "font_6_8.h"

void main() {
	int i = 0;
	
	CLK_CKDIVR = 0x00; //16mhz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	default_font = font_6_8;
	
	InitScreen();
	
	ClearScreen(screen, screen_size);
	for (i = 0; i < 84; ++i) {
		DrawChar(screen, 6, font_6_8, 6, ' ' + i, i % 14, i / 14);
	}
	UpdateScreen(screen, screen_size);
	
	pause_ds(10);
	
	i = 0;
	while (1) {
		++i;
		UpdateScreen(screen, screen_size);
		ClearScreen(screen, screen_size);
		printf("A long string.\nIteration number: %d", i);
		ResetCursor();
		//pause_ds(5);
	}
}