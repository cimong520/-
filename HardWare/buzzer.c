#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "buzzer.h"

/*  ·äÃùÆ÷  */
u8 buzzer;

void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructrue;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructrue);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void Buzzer_ON_OFF(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
}

void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}

