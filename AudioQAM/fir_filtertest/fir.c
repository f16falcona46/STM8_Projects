#include "fir.h"

const uint8_t fir_coefs[] = {
	1, 1, 1, 1, 1
};
const int fir_win_size = sizeof(fir_coefs) / sizeof(fir_coefs[0]);
uint16_t fir_window[sizeof(fir_coefs) / sizeof(fir_coefs[0])];
int fir_win_last_sample;

void insert_sample(uint16_t *fir_window, int fir_win_size, int *fir_win_last_sample, uint16_t sample)
{
	++*fir_win_last_sample;
	if (*fir_win_last_sample >= fir_win_size) {
			*fir_win_last_sample = 0;
	}
	fir_window[*fir_win_last_sample] = sample;
}

uint16_t compute_fir(const uint8_t *fir_coefs, int fir_win_size, const uint16_t *fir_window, int fir_win_last_sample)
{
	uint16_t sum = 0;
	int fir_win_index = fir_win_last_sample + 1;
	int i = 0;
	for (; i < fir_win_size; ++i) {
		if (fir_win_index >= fir_win_size) {
			fir_win_index = 0;
		}
		sum += fir_coefs[i] * fir_window[fir_win_index];
		++fir_win_index;
	}
	return sum >> FIR_COEF_RIGHT_SHIFT;
}