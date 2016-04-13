#define F_CPU 16000000L

#include <stdint.h>
#include <stdio.h>
#include <stm8/UART.h>
#include <stm8/stm8s103.h>
#include <stm8/timing.h>

void main() {
	//unsigned char low = 0;
	//unsigned char high = 0;
	unsigned char readings_raw[6];
	int readings[3];
	
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	set9600_8N1();
	set_TX(1);
	
	PD_CR1 = (1<<3); //enable push-pull on C5
	
	TIM2_ARRH = 3; //ARR = 1023
	TIM2_ARRL = 255;
	TIM2_CCER1 = 0x10; //active high, enable output
	TIM2_CCMR2 = 0x68; //PWM mode 1, preload
	TIM2_CR1 = 0x81; //enable auto reload preload, enable timer
	
	ADC_CSR = 0x03; //no extra features, up to AIN3
	ADC_CR1 = 0x03; //continuous conversion, ADON
	ADC_CR2 = 0x0A; //scan mode on, right align
	
	ADC_CR1 = 0x03; //start conversion
	while(1){
		//while (!(ADC_CSR & 0x80));
		unsigned char i = 0;
		unsigned char level = 0;
		for (i=0;i<3;++i) {
			readings_raw[(i<<1)+1] = ADC_DBxR[((i+2)<<1)+1];
			readings_raw[(i<<1)] = ADC_DBxR[((i+2)<<1)];
		}
		for (i=0;i<3;++i) {
			readings[i] = (readings_raw[(i<<1)]<<8)+readings_raw[(i<<1)+1];
		}
		level = readings[i]>>4;
		//printf("2:\t%d\t3:\t%d\t4:\t%d\r\n",readings[0],readings[1],readings[2]);
		for (i=0;i<level;++i) {
			putchar('#');
		}
		for (i=0;i<63-level;++i) {
			putchar(' ');
		}
		putchar('\r');
		putchar('\n');
		TIM2_CCR2H = readings[0]>>8;
		TIM2_CCR2L = readings[0]&0xFF;
		//pause(10);
		/*
		ADC_CSR = 0x04;
		ADC_CR1 = 0x01;
		*/
	}
}