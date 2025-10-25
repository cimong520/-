#ifndef __AD_H
#define __AD_H

// 土壤传感器ADC通道定义 (PA7对应ADC通道7)
#define SOIL_ADC_CHANNEL    7    // PA7 -> ADC_Channel_7

void AD_Init(void);

uint16_t AD_GetValue(uint8_t ADC_Channel);

u16 Get_AD_Average(uint8_t ADC_Channel,uint8_t times);

#endif
