#ifndef __DELAY_H
#define __DELAY_H

#include "../Start/stm32f10x.h"



#define Delay_1us()	{\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();\
}



void Delay_Init(void);
void Delay_us(uint64_t time);//��������
void Delay_ms(uint64_t time);
void Delay_usnop(u32 time);
void Delay_msnop(u32 time);
#define Delay_mus_nop Delay_msnop

#endif
