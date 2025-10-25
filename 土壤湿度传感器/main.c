#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"
#include "Soil.h"

/*
	AD PA5
*/

uint16_t adc;
float Humi_Soil;

int main()
{
	OLED_Init();
	AD_Init();	//浑浊度ADC通道初始化
	
	while(1)
	{		
		adc = Get_AD_Average(ADC_Channel_6, 10);
		Humi_Soil = Get_SoilHumi(ADC_Channel_6, 10);
		
		OLED_ShowNum(0,0,adc,4,OLED_8X16);
		OLED_ShowFloatNum(0,16,Humi_Soil,3,1,OLED_8X16);
		OLED_ShowChar(40,16,'%',OLED_8X16);
		OLED_Update();	
	}
}
