#define F_CPU 16000000L

#include <stm8/stm8s103.h>

//PA1 is send pin, PA2 is receive pin

void main() {
	unsigned int count = 0;
	
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	PA_DDR |= 0x02;
	PA_CR1 |= 0x02;
	
	PB_DDR |= 0x20;
	PB_CR1 |= 0x20;
	
	while (1) {
		count = 0;
		PA_ODR |= 0x02;
		while (!(PA_IDR&0x04)) ++count;
		PA_ODR &= ~0x02;
		while (PA_IDR&0x04) ++count;
		if (count>15) {
			PB_ODR |= 0x20;
		}
		else {
			PB_ODR &= ~0x20;
		}
	}
}