#ifndef __INITIALIZATION_H
#define __INITIALIZATION_H

#include "../Start/stm32f10x.h"                  // Device header
#include "../User/main.h"
#include "./task.h"
#include "./Delay.h"				//延时函数头文件
#include "../HardWare/dht11.h"
#include "../HardWare/buzzer.h"
#include "../HardWare/relay.h"
#include "../HardWare/Serial.h"
#include "../HardWare/esp8266.h"
#include "../HardWare/OLED.h"
#include "../HardWare/key.h"					//按键头文件
#include "../HardWare/LED.h"					//LED灯头文件
#include "MYRTC.h"


void Initialization(void);
void LED_Config(void);
void Key_Init(void);

void GetData(void);                        // 数据获取逻辑
void Process_Cloud_Control(void);              // 处理云端控制指令
void Menu_key_set(void)	;                 // 按键扫描逻辑
void Alarm(void);                         // 报警处理逻辑
void IWDGADD(void);             
void WIFI_Init(void);
void upload_data(void);
void SimpleDrawTempCurve(void);       // 简单的温度曲线函数
void Update_TempHuim_Display(void);     //温湿度显示函数
void Test_ESP8266(void);               //ESP8266测试
void Task_Initialization(void);;

extern char saved_mode[30] ;
extern char saved_status[20] ;


#endif
