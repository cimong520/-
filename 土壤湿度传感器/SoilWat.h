#ifndef __SOILWAT_H
#define __SOILWAT_H
#include "math.h"
#include "stm32f10x.h"

void SoilWat_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
u16 Get_AD_Average(uint8_t ADC_Channel,uint8_t times);
float SoilWat_Get(uint8_t ADC_Channel,uint8_t times);

#endif

