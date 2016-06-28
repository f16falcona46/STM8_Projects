#define F_CPU 16000000L

#include <stdint.h>
#include <stm8/stm8s.h>
#include <stm8/timing.h>
#include <stm8/UART.h>
#include <stm8/macronix_spi_cmds.h>
#include <stm8/SPI.h>
#include <stdio.h>

/*
void print_hex(uint8_t num) {
	uint8_t lowNib = num&0x0f;
	uint8_t highNib = (num&0xf0)>>4;
	if (highNib < 10) putchar('0'+highNib);
	else putchar('a'+highNib-10);
	if (lowNib < 10) putchar('0'+lowNib);
	else putchar('a'+lowNib-10);
}
*/

void main() {
	uint8_t buf[260];
	uint8_t SR;
	CLK_CKDIVR = 0x00; //16mhz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	SPI_CR2 |= (1<<1)|(1<<0); //SSM (software slave mgmt) on, SSI on (master mode)
	SPI_CR1 |= (1<<6)| (1<<4)|(1<<3) | (1<<2);  //enable spi, Fmaster/16, master mode
	
	PC_DDR |= (1<<4); //enable output on CS# line
	PC_CR1 |= (1<<4); //enable push/pull on CS# line
	PC_ODR |= (1<<4); //make CS# high to disable all chips
	
	set9600_8N1();
	set_TX(1);
	set_RX(1);
	
	while (1) {
		switch (getbyte()) {
			case 'r': //read
			{
				uint8_t len;
				uint16_t j;
				READ_cmd[1] = getbyte();
				READ_cmd[2] = getbyte();
				READ_cmd[3] = getbyte();
				len = getbyte();
				SPI_cmd(READ_cmd, READ_cmd_len, buf, len);
				for (j = 0; j < len; ++j) putchar(buf[j]);
				putchar('d');
			} break;
			case 'e': //sector erase
			{
				BE_cmd[1] = getbyte();
				BE_cmd[2] = getbyte();
				BE_cmd[3] = getbyte();
				SPI_cmd(WREN_cmd, WREN_cmd_len, 0, 0);
				SPI_cmd(BE_cmd, BE_cmd_len, 0, 0);
				do {
					SPI_cmd(RDSR_cmd, RDSR_cmd_len, &SR, 1);
				} while (SR&(0x01));
				putchar('d');
			} break;
			case 'p': //page program
			{
				uint16_t j;
				buf[0] = PP_cmd[0];
				buf[1] = getbyte();
				buf[2] = getbyte();
				buf[3] = getbyte();
				for (j = 0; j < 256; ++j) buf[j+4] = getbyte();
				SPI_cmd(WREN_cmd, WREN_cmd_len, 0, 0);
				SPI_cmd(buf, PP_cmd_len, 0, 0);
				do {
					SPI_cmd(RDSR_cmd, RDSR_cmd_len, &SR, 1);
				} while (SR&(0x01));
				putchar('d');
			} break;
			case 'q': goto done_with_commands; //quit
			default: break;
		}
	}
	done_with_commands:
	while (1);
}