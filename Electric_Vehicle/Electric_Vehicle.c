#define F_CPU 16000000L

#include <stm8/stm8s103.h>
#include <stm8/timing.h>

volatile unsigned int count = 0;
unsigned int count_max = 0;

void got_pulse() __interrupt(6) {
	++count;
	PB_ODR ^= (1<<5);
}

void main() {
	unsigned char i = 0;
	
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	PA_DDR = (1<<1)|(1<<2); //output on PA1,2
	PA_CR1 = (1<<1)|(1<<2)|(1<<3); //push-pull on PA1,2, pullup on PA3
	//TODO: find out what is necessary to get the h-bridge to stop
	
	PB_DDR = (1<<5); //output on PB5
	PB_CR1 = (1<<5); //push-pull on PB5
	
	//no outputs on port C, the outputs will be done manually
	PC_ODR = (1<<2)|(1<<3)|(1<<4)|(1<<5); //preload high on PC2,3,4,5
	PC_CR1 = (1<<6)|(1<<7); //pullup on PC6,7
	
	//no outputs on port D
	PD_CR1 = (1<<2)|(1<<3); //pullup on PC2,3
	PD_CR2 = (1<<4); //enable external interrupt on PD4
	
	PC_DDR = (1<<5);
	for (;i<4;++i) {
		pause(1);
		count_max |= (((PC_IDR&(1<<7))>>5) |
			((PC_IDR&(1<<6))>>3) |
			((PD_IDR&(1<<2))>>1) |
			((PD_IDR&(1<<3))>>3))<<(i<<2);
		PC_DDR <<= 1;
	}
	PC_DDR = 0; //read pins
	
	__asm rim __endasm; //enable interrupt
	
	while (PA_IDR & (1<<3)); //wait for start
	PA_ODR |= (1<<1); //TODO: whatever is needed to start h-bridge
	while (count<count_max);
	PA_ODR &= ~(1<<1); //TODO: whatever is needed to stop h-bridge
	
	PD_CR2 &= ~(1<<4); //disable external interrupts on PD4
	__asm halt __endasm; //halt
}