#define F_CPU 16000000L

#include <stm8/stm8s.h>
#include <stm8/timing.h>

/*
input pin
PB5

LED pin
PD7

motor pins
PA1
PA2

start button pin
PF4

configuration DIP switch
HIGH
PD3 15
PD2 14
PD0 13

PC7 12
PC6 11
PC5 10
PC4 9
PC3 8
PC2 7
PC1 6

PE5 5

PB0 4
PB1 3
PB2 2
PB3 1
PB4 0
LOW
*/



volatile unsigned int count = 0;

void got_pulse() __interrupt(EXTI1_VECT) {
	++count;
	PD_ODR ^= (1<<7);
}

void main() {
	unsigned int count_max = 0;
	
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	PA_DDR = 0x06;
	PD_CR1 = 0x06;
	
	PB_CR1 = 0x3F;
	PB_CR2 = 0x20;
	
	PC_CR1 = 0xFE;
	
	PD_DDR = 0x80;
	PD_CR1 = 0x8F;
	
	PE_CR1 = 0x20;
	
	PF_CR1 = 0x10;
	
	pause(1);
	
	while (PF_IDR & (1<<4)); //wait for start
	
	pause(1);
	
	count_max |= (PD_IDR & ((1<<3)|(1<<2)))<<(12);
	count_max |= (PD_IDR & (1<<0))<<(13);
	count_max |= (PC_IDR & (~(1<<0)))<<(5);
	count_max |= (PE_IDR & (1<<5));
	count_max |= (PB_IDR & (1<<0))<<4;
	count_max |= (PB_IDR & (1<<1))<<2;
	count_max |= (PB_IDR & (1<<2))<<0;
	count_max |= (PB_IDR & (1<<3))>>2;
	count_max |= (PB_IDR & (1<<4))>>4;
	
	__asm rim __endasm; //enable interrupt
	while (count<count_max);
	__asm halt __endasm; //halt
	
	PA_ODR &= ~(1<<1); //TODO: whatever is needed to stop h-bridge
	PB_CR2 &= ~(1<<5); //disable external interrupts on PD4
	
	while (1);
}
