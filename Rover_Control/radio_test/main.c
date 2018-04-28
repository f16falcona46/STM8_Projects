	/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include "rover_cmds.h"
#include "stm8s.h"
#include "stm8s_exti.h"
#include "stm8s_tim4.h"
#include "nrf24l01.h"
#include "shared_global_vars.h"
#include <stdlib.h>
#include <string.h>

volatile bool rf_interrupt = FALSE;
volatile bool send_message = FALSE;

uint8_t to_address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };

main()
{
	bool on = FALSE;
	nRF24L01 rf;
	
	rf.ss.port = GPIOA;
	rf.ss.pin = GPIO_PIN_1;
	rf.ce.port = GPIOA;
	rf.ce.pin = GPIO_PIN_2;
	
	GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_IN_PU_IT);
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA, EXTI_SENSITIVITY_FALL_ONLY);
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 128);
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	TIM4_Cmd(ENABLE);
	
	nRF24L01_begin(&rf);
	
	enableInterrupts();
	
	while (1) {
		if (rf_interrupt) {
			int success;
			
			rf_interrupt = FALSE;
			success = nRF24L01_transmit_success(&rf);
			if (success != 0)
				nRF24L01_flush_transmit_message(&rf);
		}

		if (send_message) {
			nRF24L01Message msg;
			
			send_message = FALSE;
			on = !on;
			if (on) memcpy(msg.data, "Hello comrade", 14);
			else memcpy(msg.data, "ON", 3);
			msg.length = strlen((char *)msg.data) + 1;
			msg.pipe_number = 0;
			nRF24L01_transmit(&rf, to_address, &msg);
		}
	}
}