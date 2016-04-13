#define F_CPU 16000000L

#include <stm8/stm8s103.h>
#include <stm8/timing.h>

void main() {
	unsigned int counter = 0;
	unsigned int count = 0; //for capacitive sensing
	unsigned int pwm_level_1 = 0;
	unsigned int pwm_level_2 = 0;
	unsigned int pwm_level_3 = 0;
	signed char pwm_dir_1 = 10;
	signed char pwm_dir_2 = 7;
	signed char pwm_dir_3 = 4;
	
	CLK_CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR1 = 0xFF; // Enable peripherals
	
	PA_DDR |= 0x02; //PA1 is send pin, PA2 is receive pin
	PA_CR1 |= 0x02;
	
	TIM2_ARRH = 3; //ARR = 1023
	TIM2_ARRL = 255;
	TIM2_CCER2 = 0x01; //active high, enable output on compare 3
	TIM2_CCMR3 = 0x68; //PWM mode 1, preload on compare 3
	TIM2_CR1 = 0x81; //enable auto reload preload, enable timer
	
	TIM1_ARRH = 3; //ARR = 1023
	TIM1_ARRL = 255;
	TIM1_CCMR3 = 0x68; //PWM mode 1, preload on compare 3
	TIM1_CCMR4 = 0x68; //PWM mode 1, preload on compare 4
	TIM1_CCER2 = 0x11; //active high, enable output on compare 3 and 4
	TIM1_BKR = 0x80; //enable all TIM1 outputs
	TIM1_CR1 = 0x81; //enable auto reload preload, enable timer
	
	while (1) {
		count = 0;
		PA_ODR |= 0x02;
		while (!(PA_IDR&0x04)) ++count;
		PA_ODR &= ~0x02;
		while (PA_IDR&0x04) ++count;
		if (count > 15) {
			for (counter = 0; counter < 400; ++counter) {
				pause(30);
				pwm_level_1 += pwm_dir_1;
				pwm_level_2 += pwm_dir_2;
				pwm_level_3 += pwm_dir_3;
				if (pwm_level_1>1000) pwm_dir_1 = -10;
				if (pwm_level_2>1000) pwm_dir_2 = -7;
				if (pwm_level_3>1000) pwm_dir_3 = -4;
				if (pwm_level_1<20) pwm_dir_1 = 10;
				if (pwm_level_2<20) pwm_dir_2 = 7;
				if (pwm_level_3<20) pwm_dir_3 = 4;
				TIM2_CCR3H = pwm_level_1>>8;
				TIM2_CCR3L = pwm_level_1&0xFF;
				TIM1_CCR3H = (1023-pwm_level_2)>>8;
				TIM1_CCR3L = (1023-pwm_level_2)&0xFF;
				TIM1_CCR4H = pwm_level_3>>8;
				TIM1_CCR4L = pwm_level_3&0xFF;
			}
		}
	}
}