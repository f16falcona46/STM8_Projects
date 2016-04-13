#define F_CPU 16000000L

#include <stdint.h>
#include <stdio.h>
#include <stm8/UART.h>
#include <stm8/stm8s103.h>
#include <stm8/timing.h>

volatile unsigned int count = 0;

const unsigned int count_max = 0;

void encoder_pulse() __interrupt(3) { //EXTI0 (Port A interrupts)
	++count;
	PB_ODR ^= 0x20; //toggle PB5
}

void main() {
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	PA_DDR = 0x0C; //output on PA2, PA3
	PA_CR1 = 0x0C; //push/pull on PA2, PA3
	PA_CR2 = 0x02; //enable interrupt on PA1
	
	PB_DDR = 0x20; //output on PB5
	PB_CR1 = 0x20;
	
	PD_CR1 = 0x10; //pull-up on PD4
	
	while(!(PD_IDR&0x10)); //wait until start button pressed
	
	PA_ODR = 0x08; //start motor
	__asm rim __endasm; //enable interrupts
	
	while(count<count_max); //wait until count is big enough
	
	__asm sim __endasm; //disable interrupts
	PA_ODR = 0x00; //stop motor
}