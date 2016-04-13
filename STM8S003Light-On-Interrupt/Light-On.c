#define F_CPU 16000000L

#include <stdint.h>
#include <stm8/stm8s003.h>
#include <stm8/timing.h>

volatile unsigned char num_blinks = 0;

void blink_isr() __interrupt(4) { //EXTI1 (port B)
	unsigned char i = 0;
	num_blinks++;
	for (;i<num_blinks;++i) {
		PD_ODR ^= 1;
		pause_ds(1);
		PD_ODR ^= 1;
		pause_ds(1);
	}
}

void main(void) {
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	
	PD_DDR = 1; //output D0
	PD_CR1 = 1; //push-pull on D0
	
	PB_CR2 = (1<<7); //enable interrupt on B7
	
	PD_ODR = 1; //set D0 to high
	
	__asm rim __endasm; //enable interrupts
	
	while(1) {
		__asm wfi __endasm;
	}
}