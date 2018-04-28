#ifndef FIR_FUNCS_H
#define FIR_FUNCS_H

#include "stm8s.h"

#define FIR_COEF_RIGHT_SHIFT 4

//fir coefs are divided by 2^4
extern const uint8_t fir_coefs[];
extern const int fir_win_size;
extern uint16_t fir_window[];
extern int fir_win_last_sample;

void insert_sample(uint16_t *fir_window, int fir_win_size, int *fir_win_last_sample, uint16_t sample);
uint16_t compute_fir(const uint8_t *fir_coefs, int fir_win_size, const uint16_t *fir_window, int fir_win_last_sample);

#endif //FIR_FUNCS_H