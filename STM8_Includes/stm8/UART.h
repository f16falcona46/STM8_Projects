#ifndef UART_UTIL_FUNCTIONS
#define UART_UTIL_FUNCTIONS

#include <stm8/stm8s.h>

int putchar(int c) {
	while(!(UART1_SR & (1 << 7)));
	UART1_DR = c;
	return c;
}

int getbyte() {
    while (!(UART1_SR & 0x20));
    return UART1_DR;
}

void set_TX(unsigned char status) {
	UART1_CR2 = (UART1_CR2&~(1<<3)) | (status<<3);
}

void set_RX(unsigned char status) {
	UART1_CR2 = (UART1_CR2&~(1<<2)) | (status<<2);
}

void set9600_8N1() {
	UART1_CR3 &= ~((1 << 4) | (1 << 5)); // 1 stop bit
	UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; // 9600 baud
}

#endif