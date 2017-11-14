#ifndef SPI_FUNCTIONS_INCLUDES
#define SPI_FUNCTIONS_INCLUDES

#include <stm8/stm8s.h>

uint8_t SPI_transfer(uint8_t output) {
	while (!(SPI_SR&(1<<1)));
	SPI_DR = output;
	while (!(SPI_SR&(1<<0)));
	output = SPI_DR;
	return output;
}

void SPI_transfer_buf(const uint8_t* source, uint8_t* dest, uint16_t len) {
	uint16_t index = 0;
	for (;index < len; ++index) {
		dest[index] = SPI_transfer(source[index]);
	}
}

void SPI_cmd(const uint8_t* cmd, uint16_t cmd_len, uint8_t* dest, uint16_t dest_len) {
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
#endif