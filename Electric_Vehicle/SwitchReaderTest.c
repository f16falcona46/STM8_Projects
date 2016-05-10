#define F_CPU 16000000L

#include <stm8/stm8s103.h>
#include <stm8/timing.h>
#include <stdio.h>
#include <stm8/UART.h>

void print_binary(unsigned int x) {
	int i = 16;
	while (i) {
		--i;
		if ((x>>i)&1) {
			putchar('1');
		}
		else {
			putchar('0');
		}
	}
}

void main() {
	unsigned int count_max = 0;
	
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	PB_CR1 = (1<<4);
	
	//no outputs on port C, the outputs will be done manually
	PC_CR1 = (1<<3)|(1<<4)|(1<<5);
	PC_CR1 = (1<<6)|(1<<7); //pullup on PC6,7
	
	//no outputs on port D
	PD_CR1 = (1<<2)|(1<<3); //pullup on PC2,3
	
	
	set9600_8N1();
	set_TX(1);
	
	while (1) {
		count_max = 0;
		PB_DDR &= ~(1<<4);
		PC_DDR &= ~((1<<3)|(1<<4)|(1<<5));
		
		PB_DDR |= (1<<4);
		pause(10);
		count_max |= (((PC_IDR&(1<<6))>>3)|
			((PC_IDR&(1<<7))>>5)|
			((PD_IDR&(1<<2))>>1)|
			((PD_IDR&(1<<3))>>3))<<12;
		PB_DDR &= ~(1<<4);
		
		PC_DDR |= (1<<3);
		pause(10);
		count_max |= (((PC_IDR&(1<<6))>>3)|
			((PC_IDR&(1<<7))>>5)|
			((PD_IDR&(1<<2))>>1)|
			((PD_IDR&(1<<3))>>3))<<8;
		PC_DDR &= ~(1<<3);
		
		PC_DDR |= (1<<4);
		pause(10);
		count_max |= (((PC_IDR&(1<<6))>>3)|
			((PC_IDR&(1<<7))>>5)|
			((PD_IDR&(1<<2))>>1)|
			((PD_IDR&(1<<3))>>3))<<4;
		PC_DDR &= ~(1<<4);
		
		PC_DDR |= (1<<5);
		pause(10);
		count_max |= (((PC_IDR&(1<<6))>>3)|
			((PC_IDR&(1<<7))>>5)|
			((PD_IDR&(1<<2))>>1)|
			((PD_IDR&(1<<3))>>3))<<0;
		PC_DDR &= ~(1<<5);
		
		//printf("%04x\r\n",count_max);
		print_binary(count_max);
		putchar('\r');
		putchar('\n');
		pause_ds(10);
	}
}