#define F_CPU 16000000L

#include <stdint.h>
#include <stdio.h>
#include <stm8/UART.h>
#include <stm8/stm8s103.h>
#include <stm8/timing.h>

unsigned char* eeprom = (volatile unsigned char*)0x4000;

void main() {
	unsigned int i = 0;
	
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	if (!(FLASH_IAPSR & 0x08)) {
		FLASH_DUKR = 0xae;
		FLASH_DUKR = 0x56;
	}
	
	PD_CR1 = 0x20;
	PD_DDR = 0x20;
	
	if (eeprom[0]) {
		eeprom[0] = 0x00;
	}
	else {
		eeprom[0] = 0x20;
	}
	
	for(;i<100;++i) {
		eeprom[i+1]=i;
	}
	
	FLASH_IAPSR &= 0xf7;
	
	PD_ODR = eeprom[0];
}