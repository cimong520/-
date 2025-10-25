#include "stm32f10x.h"                  // Device header
#include "ad.h"

float Get_SoilHumi(uint8_t ADC_Channl, uint8_t times)
{
	uint16_t adc;
	float Humi;
	
	adc = Get_AD_Average(ADC_Channl, times);
	
	Humi = (4095 - adc) * 100 / 4095;
	
	return Humi;
}
