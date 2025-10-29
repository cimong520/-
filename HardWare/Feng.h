#ifndef __Feng_H
#define __Feng_H

void Feng_Init(void);

uint16_t Feng_AD_GetValue(uint8_t ADC_Channel);
 
u16 Feng_Get_AD_Average(uint8_t ADC_Channel,uint8_t times);

float Get_FengSpeed(uint8_t ADC_Channel,uint8_t times);

#endif
