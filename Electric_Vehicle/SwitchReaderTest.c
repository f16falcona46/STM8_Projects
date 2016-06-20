#define F_CPU 16000000L

#include <stm8/stm8s.h>
#include <stm8/timing.h>
#include <stdio.h>

void putchar(char c) {
	while(!(UART2_SR & (1 << 7)));
	UART2_DR = c;
}

int getbyte() {
    while (!(UART2_SR & 0x20));
    return UART2_DR;
}

void set_TX(unsigned char status) {
	UART2_CR2 = (UART2_CR2&~(1<<3)) | (status<<3);
}

void set_RX(unsigned char status) {
	UART2_CR2 = (UART2_CR2&~(1<<2)) | (status<<2);
}

void set9600_8N1() {
	UART2_CR3 &= ~((1 << 4) | (1 << 5)); // 1 stop bit
	UART2_BRR2 = 0x03; UART2_BRR1 = 0x68; // 9600 baud
}

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
	
	PA_DDR = 0x02;
	PA_CR1 = 0x02;
	
	PB_CR1 = 0x1F;
	
	PC_CR1 = 0xFE;
	
	PD_CR1 = 0x0F;
	
	PE_CR1 = 0x20;
	
	set9600_8N1();
	set_TX(1);
	
	PA_ODR |= 0x02;
	pause_ds(5);
	PA_ODR &= ~0x02;
	
	while (1) {
		count_max = 0;
		
		count_max |= (PD_IDR & ((1<<3)|(1<<2)))<<(12);
		count_max |= (PD_IDR & (1<<0))<<(13);
		count_max |= (PC_IDR & (~(1<<0)))<<(5);
		count_max |= (PE_IDR & (1<<5));
		count_max |= (PB_IDR & (1<<0))<<4;
		count_max |= (PB_IDR & (1<<1))<<2;
		count_max |= (PB_IDR & (1<<2))<<0;
		count_max |= (PB_IDR & (1<<3))>>2;
		count_max |= (PB_IDR & (1<<4))>>4;
		
		count_max = ~count_max;
		
		print_binary(count_max);
		putchar('\r');
		putchar('\n');
		pause_ds(10);
	}
}