#ifndef __AD_H
#define __AD_H


void AD_Init(void);

uint16_t AD_GetValue(uint8_t ADC_Channel);

u16 Get_AD_Average(uint8_t ADC_Channel,uint8_t times);

#endif
