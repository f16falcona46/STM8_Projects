#include <stm8/stm8s.h>
#include <stm8/stm8s_addr.h>
#include <stm8/stm8_bitfuncs.h>

volatile unsigned char reading = 0;
volatile unsigned char psclr = 0;

unsigned char readadc(unsigned char channel) {
	ADC_CSR = (ADC_CSR&0x70) | channel;
	ADC_CR1 |= 1;
	while (!(ADC_CSR & (0x80)));
	return ADC_DRH;
}

void maybe_blink_isr() __interrupt(13) {
	if (!(++psclr)) {
		if (!(--reading)) {
			reading = readadc(3);
			PB_ODR ^= 0x20;
		}
	}
}

void main() {
	CLK_CKDIVR = 0x00;
	CLK_PCKENR1 = 0xff;
	
	PB_DDR = 0x20;
	PB_CR1 = 0x20;
	
	TIM2_CR1 = 0x80;
	TIM2_PSCR = 0x0f;
	TIM2_ARRH = 0xff;
	TIM2_ARRL = 0xff;
	TIM2_IER = 0x01;
	TIM2_CR1 |= 0x01;
	
	ADC_CR1 = 0x01;
	reading = readadc(3);
	__asm rim __endasm;
	while (1) __asm wfi __endasm;
}