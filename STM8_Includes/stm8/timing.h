#ifndef TIMING_UTIL_FUNCTIONS
#define TIMING_UTIL_FUNCTIONS

void pause(unsigned char millis) {
	unsigned long i=0;
	for (i=0; i<(F_CPU/200000)*millis; ++i);
}

void pause_ds(unsigned char decis) {
	unsigned char i = 0;
	for (i=0; i<decis; ++i) pause(100);
}

#endif