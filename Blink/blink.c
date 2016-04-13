#include <stdint.h>
#include <stm8/stm8s103.h>
#include <stm8/stm8s_addr.h>
#include <stm8/stm8_bitfuncs.h>
/*
#define CLK_DIVR	(*(volatile uint8_t *)0x50c6)
#define CLK_PCKENR1	(*(volatile uint8_t *)0x50c7)

#define TIM1_CR1	(*(volatile uint8_t *)0x5250)
#define TIM1_CNTRH	(*(volatile uint8_t *)0x525e)
#define TIM1_CNTRL	(*(volatile uint8_t *)0x525f)
#define TIM1_PSCRH	(*(volatile uint8_t *)0x5260)
#define TIM1_PSCRL	(*(volatile uint8_t *)0x5261)

#define PB_ODR	(*(volatile uint8_t *)0x5005)
#define PB_DDR	(*(volatile uint8_t *)0x5007)
#define PB_CR1	(*(volatile uint8_t *)0x5008)
*/

/*
void timer_isr() __interrupt(1) {
	
}
*/

unsigned int clock(void) {
	unsigned char h = TIM1_CNTRH;
	unsigned char l = TIM1_CNTRL;
	return((unsigned int)(h) << 8 | l);
}

void pause(unsigned int millis) {
	unsigned int start = clock();
	while (clock() < start + millis);
}

void main(void) {
	unsigned char i = 0;
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz

	// Configure timer
	// 1000 ticks per second
	TIM1_PSCRH = 0x3e;
	TIM1_PSCRL = 0x80;
	// Enable timer
	TIM1_CR1 = 0x01;

	TIM2_PSCR = 0x00;       //  Prescaler = 1.
	TIM2_ARRH = 0xc3;       //  High byte of 50,000.
	TIM2_ARRL = 0x50;       //  Low byte of 50,000.
	TIM2_CCR1H = 0x30;      //  High byte of 12,500
	TIM2_CCR1L = 0xd4;      //  Low byte of 12,500
	TIM2_CCER1 = ((TIM2_CCER1)&(0xF6))|0x01;
	//TIM2_CCER1_CC1P = 0;    //  Active high.
	//TIM2_CCER1_CC1E = 1;    //  Enable compare mode for channel 1
	TIM2_CCMR1 = ((TIM2_CCMR1)&0x8F)|0x60;
	//TIM2_CCMR1_OC1M = 6;    //  PWM Mode 1 - active if counter < CCR1, inactive otherwise.
	TIM2_CR1 |= 1;       //  Finally enable the timer.
	
	PB_DDR = 0x20;
	PB_CR1 = 0x20;
	
	PC_DDR = 1<<5;
	PC_CR1 = 1<<5;
	
	PA_DDR = 1<<3;
	__asm;
rim 
__endasm;
	
	while (1) {
		//PB_ODR ^= 0x20;
		TOGGLEBIT(PB_ODR_ADDR,5);
		//__asm bcpl PB_ODR_ADDR,#5 __endasm;
		pause(500);
	}
}