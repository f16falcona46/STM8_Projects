#define F_CPU 16000000L

#include <stm8/stm8s103.h>
#include <stm8/timing.h>
#include <stdio.h>
#include <stm8/UART.h>

void main() {
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	set9600_8N1();
	set_TX(1);
	printf("test\n");
	while (1);
}