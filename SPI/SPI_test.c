#define F_CPU 16000000L

#include <stdint.h>
#include <stm8/stm8s.h>
#include <stm8/timing.h>
#include <stm8/UART.h>
#include <stm8/macronix_spi_cmds.h>
#include <stm8/SPI.h>
#include <stdio.h>

void print_hex(uint8_t num) {
	uint8_t lowNib = num&0x0f;
	uint8_t highNib = (num&0xf0)>>4;
	if (highNib < 10) putchar('0'+highNib);
	else putchar('a'+highNib-10);
	if (lowNib < 10) putchar('0'+lowNib);
	else putchar('a'+lowNib-10);
}

void main() {
	uint8_t buf[260];
	uint8_t SR;
	int i;
	CLK_CKDIVR = 0x00; //16mhz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	SPI_CR2 |= (1<<1)|(1<<0); //SSM (software slave mgmt) on, SSI on (master mode)
	SPI_CR1 |= (1<<6)| (1<<4)|(1<<3) | (1<<2);  //enable spi, Fmaster/16, master mode
	
	PC_DDR |= (1<<4); //enable output on CS# line
	PC_CR1 |= (1<<4); //enable push/pull on CS# line
	PC_ODR |= (1<<4); //make CS# high to disable all chips
	
	set9600_8N1();
	set_TX(1);
	
	SPI_cmd(WREN_cmd, WREN_cmd_len, 0, 0);
	printf("Starting chip erase...\r\n");
	SPI_cmd(CE_cmd, CE_cmd_len, 0, 0);
	do {
		SPI_cmd(RDSR_cmd, RDSR_cmd_len, &SR, 1);
	} while (SR&(0x01));
	printf("Done.\r\n");
	
	printf("Writing 0-255...\r\n");
	buf[0] = PP_cmd[0];
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;
	SR = 0;
	do {
		buf[SR+4] = SR<<1;
		++SR;
	} while (SR);
	SPI_cmd(WREN_cmd, WREN_cmd_len, 0, 0);
	SPI_cmd(buf, PP_cmd_len, 0, 0);
	do {
		SPI_cmd(RDSR_cmd, RDSR_cmd_len, &SR, 1);
	} while (SR&(0x01));
	printf("Done.\r\n");
	
	printf("Reading...\r\n");
	READ_cmd[1] = 0;
	READ_cmd[2] = 0;
	READ_cmd[3] = 0;
	SPI_cmd(READ_cmd, READ_cmd_len, buf, 256);
	for (i = 0; i < 256; ++i) {
		print_hex(i);
		putchar(' ');
		print_hex(buf[i]);
		putchar('\r');
		putchar('\n');
	}
	printf("Done.\r\n");
	
	while (1);
}