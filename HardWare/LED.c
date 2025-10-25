#include "../System/Initialization_Data.h"
#include "./LED.h"

 void LED_ON(void)
{
	GPIO_ResetBits(LED_GPIO,LED_PIN);
}

void LED_OFF(void)
{
	GPIO_SetBits(LED_GPIO,LED_PIN);
}
void LED_Turn(void)
{
	if(GPIO_ReadInputDataBit(LED_GPIO,LED_PIN)==0)
	{
		GPIO_SetBits(LED_GPIO,LED_PIN);
	}
	else
	{
		GPIO_ResetBits(LED_GPIO,LED_PIN);
	}
}
