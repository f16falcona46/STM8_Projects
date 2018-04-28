#define F_CPU 16000000L

#include <iostm8s103.h>

#include "stm8s_tim2.h"
#include "stm8s_adc1.h"

void assert_failed(uint8_t* file, uint32_t line)
{
	while (1); //give up
}

int main()
{
	TIM2_DeInit();
	TIM2_Cmd(ENABLE);
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 1000);
	TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 100, TIM2_OCPOLARITY_HIGH);
	TIM2_CCxCmd(TIM2_CHANNEL_1, ENABLE);
	ADC1_DeInit();
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_GPIO, DISABLE, ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_CHANNEL0, DISABLE);
	while (1) {
		ADC1_StartConversion();
		while (!ADC1_GetFlagStatus(ADC1_FLAG_EOC));
		uint16_t conv = ADC1_GetConversionValue();
		nop();
	}
}