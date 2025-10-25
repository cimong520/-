#include "stm32f10x.h"                  // Device header

void relay_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	 
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void relay_OFF(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void relay_ON(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}

void relay_Ture(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
}

