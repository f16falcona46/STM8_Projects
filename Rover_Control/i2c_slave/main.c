/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include "stm8s.h"

main()
{
	CLK->CKDIVR = 0x00;
	I2C->CR1 &= ~I2C_CR1_PE;
	I2C->FREQR = 16;
	I2C->CCRL = 0xa0;                    //  SCL clock speed is 50 KHz.
	I2C->CCRH = 0x00;
	I2C->OARH = I2C_OARH_ADDCONF;
	I2C->OARL = 0x1d << 1;
	I2C->TRISER = 17;
	I2C->ITR = I2C_ITR_ITBUFEN | I2C_ITR_ITEVTEN | I2C_ITR_ITERREN;
	I2C->CR1 |= I2C_CR1_PE;
	I2C->CR2 = I2C_CR2_ACK;
	enableInterrupts();
	
	while (1);
}