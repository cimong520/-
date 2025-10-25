#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Feng.h"

uint16_t adc;
float speed;

/*
	AD PA5
*/

int main()
{
	OLED_Init();
	Feng_Init();	//风速ADC通道初始化
	
	while(1)
	{
		adc = Get_AD_Average(ADC_Channel_5,10);	//读取ADC通道的AD值
		speed = Get_FengSpeed(ADC_Channel_5,10);	//读取风速的速度值
		OLED_ShowNum(0,0,adc,4,OLED_8X16);
		OLED_ShowFloatNum(0,16,speed,2,1,OLED_8X16);
		OLED_Update();	
	}
}
