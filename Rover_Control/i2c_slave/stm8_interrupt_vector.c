/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */

#include "stm8s.h"

typedef void @far (*interrupt_handler_t)(void);

typedef enum {
	WAIT_FOR_ADDR,
	RW} RegisterState;

#define SIZE 16
u8 addr = 0;
u8 reg_state = WAIT_FOR_ADDR;
u8 data[SIZE] = {0};

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

@far @interrupt void I2C_Interrupt(void)
{
	u8 sr1 = I2C->SR1;
	u8 sr2 = I2C->SR2;
	if (sr1 & I2C_SR1_ADDR) {
		I2C->SR1;
		I2C->SR3;
		reg_state = WAIT_FOR_ADDR;
	}
	if (sr1 & I2C_SR1_RXNE) {
		if (reg_state == WAIT_FOR_ADDR) {
			addr = I2C->DR;
			reg_state = RW;
		}
		else if (reg_state == RW) {
			data[addr] = I2C->DR;
			++addr;
			if (addr >= SIZE) addr = 0;
		}
	}
	if (sr1 & I2C_SR1_TXE) {
		I2C->DR = data[addr];
		++addr;
		if (addr >= SIZE) addr = 0;
	}
	if (sr2 & I2C_SR2_AF) {
		I2C->SR2 &= ~I2C_SR2_AF;
		if (addr == 0) {
			addr = SIZE - 1;
		}
		else {
			--addr;
		}
	}
	if (sr1 & I2C_SR1_STOPF) {
		I2C->SR1;
		I2C->CR2 &= ~I2C_CR2_STOP;
	}
	return;
}

extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, NonHandledInterrupt}, /* irq1  */
	{0x82, NonHandledInterrupt}, /* irq2  */
	{0x82, NonHandledInterrupt}, /* irq3  */
	{0x82, NonHandledInterrupt}, /* irq4  */
	{0x82, NonHandledInterrupt}, /* irq5  */
	{0x82, NonHandledInterrupt}, /* irq6  */
	{0x82, NonHandledInterrupt}, /* irq7  */
	{0x82, NonHandledInterrupt}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, NonHandledInterrupt}, /* irq10 */
	{0x82, NonHandledInterrupt}, /* irq11 */
	{0x82, NonHandledInterrupt}, /* irq12 */
	{0x82, NonHandledInterrupt}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, NonHandledInterrupt}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, NonHandledInterrupt}, /* irq18 */
	{0x82, I2C_Interrupt}, /* irq19 (I2C interrupt)*/
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, NonHandledInterrupt}, /* irq22 */
	{0x82, NonHandledInterrupt}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
