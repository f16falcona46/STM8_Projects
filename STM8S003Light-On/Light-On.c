#include <stdint.h>
#include <stm8/stm8s003.h>

void main(void) {
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	
	PD_DDR = 1;
	PD_CR1 = 1;
	
	while(1) {
		//unsigned int i = 0;
		PD_ODR = (PB_IDR&0x80)?1:0;
		//for(; i <100;++i) {
		//	unsigned int j = 0;
		//	for (;j<10000;++j);
		//}
	}
}