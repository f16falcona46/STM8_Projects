#define F_CPU 16000000L

#include <stdint.h>
#include <stm8/stm8s.h>
#include <stm8/timing.h>
#include <stm8/UART.h>
#include <stdio.h>

const uint8_t RDID_cmd[] = {0x9f};

uint8_t SPI_transfer(uint8_t output) {
	while (!(SPI_SR&(1<<1)));
	SPI_DR = output;
	while (!(SPI_SR&(1<<1)));
	output = SPI_DR;
	return output;
}

void SPI_transfer_buf(uint8_t* source, uint8_t* dest, uint16_t len) {
	uint16_t index = 0;
	for (;index < len; ++index) {
		dest[index] = SPI_transfer(source[index]);
	}
}

void SPI_cmd(uint8_t* cmd, uint16_t cmd_len, uint8_t* dest, uint16_t dest_len) {
	uint16_t index = 0;
	PC_ODR &= ~(1<<4); //assert CS# low
	for (;index < cmd_len; ++index) {
		SPI_transfer(cmd[index]);
	}
	for (index = 0; index < dest_len; ++index) {
		dest[index] = SPI_transfer(0);
	}
	PC_ODR |= (1<<4); //release CS# (make it high)
}

void main() {
	uint8_t buf[3];
	CLK_CKDIVR = 0x00; //16mhz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	SPI_CR2 |= (1<<1)|(1<<0); //SSM (software slave mgmt) on, SSI on (master mode)
	SPI_CR1 |= (1<<6)| (1<<4)|(1<<3) | (1<<2);  //enable spi, Fmaster/16, master mode
	
	PC_DDR |= (1<<4); //enable output on CS# line
	PC_CR1 |= (1<<4); //enable push/pull on CS# line
	PC_ODR |= (1<<4); //make CS# high to disable all chips
	
	set9600_8N1();
	set_TX(1);
	putchar('a');
	while (1) {
		SPI_cmd(RDID_cmd, 1, buf, 3);
		printf("%02x\r\n", buf[0]);
		pause_ds(1);
	}
}