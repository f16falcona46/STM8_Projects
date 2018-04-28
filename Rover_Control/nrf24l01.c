	#include <string.h>
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"

#include "stm8s.h"
#include "stm8s_gpio.h"
#include "stm8s_spi.h"

#ifdef __CSMC__
#define inline @inline
#endif

#ifndef _BV
#define _BV(shift) (1<<(shift))
#endif

static void copy_address(uint8_t *source, uint8_t *destination);
inline static void set_as_output(gpio_pin pin)
{
	GPIO_Init(pin.port, pin.pin, GPIO_MODE_OUT_PP_HIGH_FAST);
}

inline static void set_as_input(gpio_pin pin) {
	GPIO_Init(pin.port, pin.pin, GPIO_MODE_IN_FL_NO_IT);
}

inline static void set_high(gpio_pin pin)
{
	GPIO_WriteHigh(pin.port, pin.pin);
}

inline static void set_low(gpio_pin pin)
{
	GPIO_WriteLow(pin.port, pin.pin);
}

static void spi_init(nRF24L01 *rf);
static uint8_t spi_transfer(uint8_t data);

void nRF24L01_begin(nRF24L01 *rf) {
	uint8_t data;
	
	set_as_output(rf->ss);
	set_as_output(rf->ce);

	set_high(rf->ss);
	set_low(rf->ce);

	spi_init(rf);

	nRF24L01_send_command(rf, FLUSH_RX, NULL, 0);
	nRF24L01_send_command(rf, FLUSH_TX, NULL, 0);
	nRF24L01_clear_interrupts(rf);

	data = _BV(EN_CRC) | _BV(CRCO) | _BV(PWR_UP) | _BV(PRIM_RX);
	nRF24L01_write_register(rf, CONFIG, &data, 1);

	// enable Auto Acknowlegde on all pipes
	data = _BV(ENAA_P0) | _BV(ENAA_P1) | _BV(ENAA_P2)
		 | _BV(ENAA_P3) | _BV(ENAA_P4) | _BV(ENAA_P5);
	nRF24L01_write_register(rf, EN_AA, &data, 1);

	// enable Dynamic Payload on al pipes
	data = _BV(DPL_P0) | _BV(DPL_P1) | _BV(DPL_P2)
		 | _BV(DPL_P3) | _BV(DPL_P4) | _BV(DPL_P5);
	nRF24L01_write_register(rf, DYNPD, &data, 1);

	// enable Dynamic Payload (global)
	data = _BV(EN_DPL);
	nRF24L01_write_register(rf, FEATURE, &data, 1);

	// disable all rx addresses
	data = 0;
	nRF24L01_write_register(rf, EN_RXADDR, &data, 1);
}

uint8_t nRF24L01_send_command(nRF24L01 *rf, uint8_t command, void *data,
	size_t length) {
	unsigned int i;
	
	set_low(rf->ss);

	rf->status = spi_transfer(command);
	for (i = 0; i < length; i++)
		((uint8_t*)data)[i] = spi_transfer(((uint8_t*)data)[i]);

	set_high(rf->ss);

	return rf->status;
}

uint8_t nRF24L01_write_register(nRF24L01 *rf, uint8_t reg_address, void *data,
	size_t length) {
	return nRF24L01_send_command(rf, W_REGISTER | reg_address, data, length);
}

uint8_t nRF24L01_read_register(nRF24L01 *rf, uint8_t reg_address, void *data,
	size_t length) {
	return nRF24L01_send_command(rf, R_REGISTER | reg_address, data, length);
}

uint8_t nRF24L01_no_op(nRF24L01 *rf) {
	return nRF24L01_send_command(rf, NOP, NULL, 0);
}

uint8_t nRF24L01_update_status(nRF24L01 *rf) {
	return nRF24L01_no_op(rf);
}

uint8_t nRF24L01_get_status(nRF24L01 *rf) {
	return rf->status;
}

bool nRF24L01_data_received(nRF24L01 *rf) {
	set_low(rf->ce);
	nRF24L01_update_status(rf);
	return nRF24L01_pipe_number_received(rf) >= 0;
}

void nRF24L01_listen(nRF24L01 *rf, int pipe, uint8_t *address) {
	uint8_t addr[5];
	uint8_t current_pipes;
	
	copy_address(address, addr);

	nRF24L01_write_register(rf, RX_ADDR_P0 + pipe, addr, 5);

	nRF24L01_read_register(rf, EN_RXADDR, &current_pipes, 1);
	current_pipes |= _BV(pipe);
	nRF24L01_write_register(rf, EN_RXADDR, &current_pipes, 1);

	set_high(rf->ce);
}

bool nRF24L01_read_received_data(nRF24L01 *rf, nRF24L01Message *message) {
	message->pipe_number = nRF24L01_pipe_number_received(rf);
	nRF24L01_clear_receive_interrupt(rf);
	if (message->pipe_number < 0) {
		message->length = 0;
		return FALSE;
	}

	nRF24L01_read_register(rf, R_RX_PL_WID, &message->length, 1);

	if (message->length > 0) {
		nRF24L01_send_command(rf, R_RX_PAYLOAD, &message->data,
			message->length);
	}

	return TRUE;
}

int nRF24L01_pipe_number_received(nRF24L01 *rf) {
	int pipe_number = (rf->status & RX_P_NO_MASK) >> 1;
	return pipe_number <= 5 ? pipe_number : -1;
}

void nRF24L01_transmit(nRF24L01 *rf, void *address, nRF24L01Message *msg) {
	uint8_t addr[5];
	uint8_t config;
	
	nRF24L01_clear_transmit_interrupts(rf);
	copy_address((uint8_t *)address, addr);
	nRF24L01_write_register(rf, TX_ADDR, addr, 5);
	copy_address((uint8_t *)address, addr);
	nRF24L01_write_register(rf, RX_ADDR_P0, addr, 5);
	nRF24L01_send_command(rf, W_TX_PAYLOAD, &msg->data, msg->length);
	nRF24L01_read_register(rf, CONFIG, &config, 1);
	config &= ~_BV(PRIM_RX);
	nRF24L01_write_register(rf, CONFIG, &config, 1);
	set_high(rf->ce);
}

int nRF24L01_transmit_success(nRF24L01 *rf) {
	int success;
	uint8_t config;
	
	set_low(rf->ce);
	nRF24L01_update_status(rf);
	if (rf->status & _BV(TX_DS)) success = 0;
	else if (rf->status & _BV(MAX_RT)) success = -1;
	else success = -2;
	nRF24L01_clear_transmit_interrupts(rf);
	nRF24L01_read_register(rf, CONFIG, &config, 1);
	config |= _BV(PRIM_RX);
	nRF24L01_write_register(rf, CONFIG, &config, 1);
	return success;
}

void nRF24L01_flush_transmit_message(nRF24L01 *rf) {
	nRF24L01_send_command(rf, FLUSH_TX, NULL, 0);
}

void nRF24L01_retry_transmit(nRF24L01 *rf) {
	// XXX not sure it works this way, never tested
	uint8_t config;
	nRF24L01_read_register(rf, CONFIG, &config, 1);
	config &= ~_BV(PRIM_RX);
	nRF24L01_write_register(rf, CONFIG, &config, 1);
	set_high(rf->ce);
}

void nRF24L01_clear_interrupts(nRF24L01 *rf) {
	uint8_t data = _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT);
	nRF24L01_write_register(rf, STATUS, &data, 1);
}

void nRF24L01_clear_transmit_interrupts(nRF24L01 *rf) {
	uint8_t data = _BV(TX_DS) | _BV(MAX_RT);
	nRF24L01_write_register(rf, STATUS, &data, 1);
}

void nRF24L01_clear_receive_interrupt(nRF24L01 *rf) {
	uint8_t data = _BV(RX_DR) | rf->status;
	nRF24L01_write_register(rf, STATUS, &data, 1);
}

static void copy_address(uint8_t *source, uint8_t *destination) {
	int i;
	for (i = 0; i < 5; i++)
		destination[i] = source[i];
}

static void spi_init(nRF24L01 *rf) {
	SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0);
	SPI_Cmd(ENABLE);
}

static uint8_t spi_transfer(uint8_t data) {
	SPI_SendData(data);
	while (!(SPI_GetFlagStatus(SPI_FLAG_RXNE)));
	return SPI_ReceiveData();
}
