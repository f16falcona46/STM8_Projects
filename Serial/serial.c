#include <stdint.h>
#include <stdio.h>
#include <stm8/UART.h>
#include <stm8/stm8s103.h>

void main() {
	unsigned int i = 0;
	const char* message = "lol";
	//float f = 1;

	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	set9600_8N1();
	set_TX(1);
	
	for(;i<1024;++i){
		/*
		printf("Hello World!\r\n%d,%d\r\n",*(int*)(&f),*((int*)(&f)+1));
		f+=0.1;
		for(i = 0; i < 147456; i++); // Sleep
		*/
		printf("%d\r\n",*((unsigned char*)i));
	}
	while (message[i]) {
		while(!(UART1_SR & (1 << 7)));
		UART1_DR = message[i];
	}
	while (1);
}