#ifndef SHARED_GLOBAL_VARS_H
#define SHARED_GLOBAL_VARS_H

#include "stm8s.h"

extern volatile bool rf_interrupt;
extern volatile bool send_message;
extern volatile uint8_t send_message_ctr;

#endif //SHARED_GLOBAL_VARS_H