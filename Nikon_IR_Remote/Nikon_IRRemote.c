#define F_CPU 16000000L

#include <stdint.h>
#include <stm8/stm8s003.h>
#include <stm8/timing.h>

const uint16_t PulseWidths[] = { 76, 1064, 15, 60, 15, 136, 15, 2402, 76, 1064, 15, 60, 15, 136, 15 };
const uint8_t PulseWidths_len = 15;

uint16_t PartOfCycle = 0;
uint8_t CyclePtr = 0;
uint8_t Enabled = 0;

void Timer_ISR() __interrupt(11) //TIM1 update interrupt
{
	if (1) {
		++PartOfCycle;
		if (PartOfCycle > PulseWidths[CyclePtr]) {
			++CyclePtr;
			PartOfCycle = 0;
			if (CyclePtr < PulseWidths_len) {
				TIM1_CCER2 ^= (1<<0); //toggle timer output
			}
			else {
				TIM1_CR1 &= ~(1<<0); //disable timer
				TIM1_CCER2 &= ~(1<<0); //disable timer output
				CyclePtr = 0;
				PartOfCycle = 0;
				Enabled = 0;
				PB_ODR ^= (1<<5); //toggle B5
			}
		}
	}
	TIM1_SR1 = 0;
}

int main()
{
	CLK_CKDIVR = 0x00; //16mhz
	CLK_PCKENR1 |= (1<<7); // Enable clock for TIM1
	
	TIM1_ARRH = (421 & 0xff00) >> 8; //16 MHz / 38 KHz = 421
	TIM1_ARRL = 421 & 0x00ff;
	
	TIM1_CCR3H = ((421 / 2) & 0xff00) >> 8; //duty cycle = 50%
	TIM1_CCR3L = (421 / 2) & 0x00ff;
	
	TIM1_CR1 = (1<<4) | (1<<2); //no preload, count down, interrupt on underflow only
	TIM1_IER = (1<<0); //enable interrupt on update event
	TIM1_BKR |= (1<<7); //enable all outputs
	
	PB_DDR |= (1<<5); //enable output push-pull on B5
	PB_CR1 |= (1<<5);
	
	PC_DDR |= (1<<3); //enable output push-pull on C3
	PC_CR2 |= (1<<3);
	
	__asm rim __endasm;
	
	
	TIM1_CCMR3 = (1<<6) | (1<<5) | (0<<4); //PWM mode 1 (ON if count < CCR)
	TIM1_CCER2 |= (1<<0); //enable timer output
	TIM1_CR1 |= (1<<0); //enable timer
	
	while (1) {
		if ((PA_IDR & (1<<1)) && !Enabled) {
			PartOfCycle = 0;
			CyclePtr = 0;
			Enabled = 1;
			TIM1_CR1 |= (1<<0); //enable timer
			TIM1_CCER2 |= (1<<0); //enable timer output
		}
		else if (!(PA_IDR & (1<<1))) {
			Enabled = 0;
			TIM1_CR1 &= ~(1<<0); //disable timer
			TIM1_CCER2 &= ~(1<<0); //disable timer output
		}
		pause_ds(10);
	}
}