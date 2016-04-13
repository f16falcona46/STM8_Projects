#include <stdint.h>
#include <stdio.h>
#include <stm8/UART.h>
#include <stm8/stm8s103.h>

void float_add(unsigned char* accum, unsigned char* addend) {
	unsigned char sign_accum = accum[0]>>7;
	unsigned char sign_addend = addend[0]>>7;
	unsigned int exp = ((int)accum[0]<<1)+((accum[1]&0x80)>>7);
	int exp_diff = (((int)accum[0]<<1)+((accum[1]&0x80)>>7))-((addend[0]<<1)+((addend[1]&0x80)>>7));
	signed long mantissa_accum = 0x00800000 | ((signed long)accum[1]&0x7f)<<16 | (signed long)accum[2]<<8 | accum[3];
	signed long mantissa_addend = 0x00800000 | ((signed long)addend[1]&0x7f)<<16 | (signed long)addend[2]<<8 | addend[3];
	unsigned char check_sub_normalize = 0;
	
	if (sign_accum == sign_addend) {
		if (exp_diff>0) {
			mantissa_accum += mantissa_addend>>(exp_diff);
		}
		else {
			mantissa_accum = mantissa_addend + (mantissa_accum>>exp_diff);
		}
	}
	else {
		if (exp_diff>0) {
			mantissa_accum -= mantissa_addend>>(exp_diff);
		}
		else if (exp_diff < 0) {
			mantissa_accum = mantissa_addend - (mantissa_accum>>exp_diff);
			sign_accum = sign_addend;
		}
		else {
			check_sub_normalize = 1;
			mantissa_accum -= mantissa_addend;
		}
	}
	if (mantissa_accum&0x01000000) exp+=1;
	if (mantissa_accum<0) {
		sign_accum = sign_addend;
	}
	if (check_sub_normalize) {
		while (!(0x00800000&mantissa_accum)) {
			mantissa_accum = mantissa_accum << 1;
			exp -=1;
		}
	}
	printf("Mantissa:%ld\r\n",mantissa_accum);
	printf("Exponent:%u\r\n",exp+exp_diff);
	printf("Sign:%u\r\n",sign_accum);
	
	accum[0] = (sign_accum<<7) | ((exp+exp_diff)>>1);
	accum[1] = ((exp+exp_diff)<<7) | (0xff&((0x007fffff&mantissa_accum)>>16));
	accum[2] = (0xff&((0x007fffff&mantissa_accum)>>8));
	accum[3] = (0xff&mantissa_accum);
}

void main() {
	float auto1 = 3.23;
	float auto2 = 5.81;
	float auto3 = 0;
	int manual1[2] = {16462,-18350};
	int manual2[2] = {16569,-5243};
	int manualL = 0;
	int manualH = 0;

	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	set9600_8N1();
	set_TX(1);
	
	auto3 = auto1+auto2;
	
	float_add((unsigned char*)manual1,(unsigned char*)manual2);
	
	manualH = manual1[0];
	manualL = manual1[1];
	
	printf("Auto1:\t%d\t%d\r\n",*(int*)(&auto1),*((int*)(&auto1)+1));
	printf("Auto2:\t%d\t%d\r\n",*(int*)(&auto2),*((int*)(&auto2)+1));
	printf("Auto3:\t%d\t%d\r\n",*(int*)(&auto3),*((int*)(&auto3)+1));
	printf("Manual:\t%d\t%d\r\n",*(int*)(&manualH),*((int*)(&manualL)+1));
	
	while(1);
}