#define F_CPU 16000000L

#include <stm8/stm8s.h>
#include <stm8/timing.h>

volatile unsigned int count = 0;

void got_pulse() __interrupt(EXTI1_VECT) {
	++count;
	PB_ODR ^= (1<<5);
}

void main() {
	unsigned int count_max = 0;
	
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	//init port A
	//init port B
	//init port C
	//init port D
	//init port E
	
	count_max |= (PD_IDR & ((1<<3)|(1<<2)))<<(12);
	count_max |= (PD_IDR & (1<<0))<<(13);
	count_max |= (PC_IDR & (~(1<<0)))<<(5);
	count_max |= (PE_IDR & (1<<5));
	count_max |= (PB_IDR & (1<<0))<<4;
	count_max |= (PB_IDR & (1<<1))<<2;
	count_max |= (PB_IDR & (1<<2))<<0;
	count_max |= (PB_IDR & (1<<3))>>2;
	count_max |= (PB_IDR & (1<<4))>>4;
	
	while (PA_IDR & (1<<3)); //wait for start
	
	
	__asm rim __endasm; //enable interrupt
	while (count<count_max);
	__asm halt __endasm; //halt
	
	PA_ODR &= ~(1<<1); //TODO: whatever is needed to stop h-bridge
	PD_CR2 &= ~(1<<4); //disable external interrupts on PD4
	
	while (1);
}
