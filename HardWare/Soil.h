#ifndef __Soil_H_
#define __Soil_H_

// 土壤湿度传感器ADC通道定义 (PA7对应ADC通道7)
#define SOIL_HUMI_ADC_CHANNEL    7    // PA7 -> ADC_Channel_7

float Get_SoilHumi(uint8_t ADC_Channl, uint8_t times);

#endif
