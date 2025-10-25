#include "stm32f10x.h"                  // Device header
#include <time.h>

void MyRTC_SetTime(void);
uint16_t MyRTC_Time[]={2024,9,12,01,16,00};				//时钟初始时间

void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	RCC_LSICmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)!=SET);
	
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		RCC_RTCCLKCmd(ENABLE);
	
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		
		RTC_SetPrescaler(40000-1);
		RTC_WaitForLastTask();
	
		MyRTC_SetTime();

}
//设置时间
void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_data;
	
	time_data.tm_year=MyRTC_Time[0]-1900;
	time_data.tm_mon=MyRTC_Time[1]-1;
	time_data.tm_mday=MyRTC_Time[2];
	time_data.tm_hour=MyRTC_Time[3];
	time_data.tm_min=MyRTC_Time[4];
	time_data.tm_sec=MyRTC_Time[5];

	time_cnt = mktime(&time_data)-8*60*60;
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();

}
//读取时间
void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_data;
	time_cnt=RTC_GetCounter()+8*60*60;
	time_data=*localtime(&time_cnt);
	MyRTC_Time[0]=time_data.tm_year+1900;
	MyRTC_Time[1]=time_data.tm_mon+1;
	MyRTC_Time[2]=time_data.tm_mday;
	MyRTC_Time[3]=time_data.tm_hour;
	MyRTC_Time[4]=time_data.tm_min;
	MyRTC_Time[5]=time_data.tm_sec;

}
