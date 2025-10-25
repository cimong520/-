#ifndef __INITIALIZATION_H
#define __INITIALIZATION_H

#include "../Start/stm32f10x.h"                  // Device header
#include "../User/main.h"
#include "./task.h"
#include "./Delay.h"				//延时函数头文件
#include "../HardWare/oled.h"					//OLED屏头文件
#include "../HardWare/key.h"					//按键头文件
#include "../HardWare/LED.h"					//LED灯头文件
#include "MYRTC.h"
#include "esp8266.h"

void Initialization(void);
void LED_Config(void);
void Key_Init(void);

#endif
