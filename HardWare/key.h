#ifndef __KEY_H
#define __KEY_H

#include "../Start/stm32f10x.h"

void Key_Init(void);
uint8_t Get_Key_1(void);
uint8_t Get_Key_2(void);
uint8_t Get_Key_3(void);
uint8_t Get_Key_4(void);
uint8_t Key_Scan(void);

#endif
