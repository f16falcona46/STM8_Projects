#ifndef SPI_FUNCTIONS_INCLUDES
#define SPI_FUNCTIONS_INCLUDES

#include <stdint.h>

uint8_t SPI_transfer(uint8_t output);

void SPI_transfer_buf(const uint8_t* source, uint8_t* dest, uint16_t len);

void SPI_cmd(const uint8_t* cmd, uint16_t cmd_len, uint8_t* dest, uint16_t dest_len);
#endif