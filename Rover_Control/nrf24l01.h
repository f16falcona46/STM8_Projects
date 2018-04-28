#ifndef _NRF24L01_H
#define _NRF24L01_H

#include <stdlib.h>
#include "stm8s_gpio.h"

typedef struct {
	int pipe_number;
	uint8_t data[32];
	uint8_t length;
} nRF24L01Message;

typedef struct {
	GPIO_TypeDef *port;
	GPIO_Pin_TypeDef pin;
} gpio_pin;

typedef struct {
	gpio_pin ss; // slave select
	gpio_pin ce; // chip enabled
	uint8_t status;
} nRF24L01;


void nRF24L01_begin(nRF24L01 *rf);
uint8_t nRF24L01_send_command(nRF24L01 *rf, uint8_t command, void *data, size_t length);
uint8_t nRF24L01_write_register(nRF24L01 *rf, uint8_t reg_address, void *data, size_t length);
uint8_t nRF24L01_read_register(nRF24L01 *rf, uint8_t regAddress, void *data, size_t length);
uint8_t nRF24L01_no_op(nRF24L01 *rf);
uint8_t nRF24L01_update_status(nRF24L01 *rf);
uint8_t nRF24L01_get_status(nRF24L01 *rf);
void nRF24L01_listen(nRF24L01 *rf, int pipe, uint8_t *address);
bool nRF24L01_data_received(nRF24L01 *rf);
bool nRF24L01_read_received_data(nRF24L01 *rf, nRF24L01Message *message);
int nRF24L01_pipe_number_received(nRF24L01 *rf);
void nRF24L01_transmit(nRF24L01 *rf, void *address, nRF24L01Message *msg);
int nRF24L01_transmit_success(nRF24L01 *rf);
void nRF24L01_flush_transmit_message(nRF24L01 *rf);
void nRF24L01_retry_transmit(nRF24L01 *rf);
void nRF24L01_clear_interrupts(nRF24L01 *rf);
void nRF24L01_clear_transmit_interrupts(nRF24L01 *rf);
void nRF24L01_clear_receive_interrupt(nRF24L01 *rf);

#endif
