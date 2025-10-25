#include "stm32f10x.h"		//STM32头文件
#include "Delay.h"				//延时函数头文件
#include "math.h"
#include "oled.h"					//OLED屏头文件
#include "key.h"					//按键头文件
#include "LED.h"					//LED灯头文件
#include "relay.h"					//继电器头文件	



uint16_t i; 			//i页面标志位
uint8_t key1=0,key2=0,key3=0; //按键标志位


void ShowData(void);    //数据页面
void GetData(void);			//获取数据
void Alarm(void);				//自动控制函数
void Menu_key_set(void);//按键扫描
void Key_Mune(void);		//按键切换页面操作

int main(void)
{	
	SystemInit();  
	Delay_Init();					//滴答定时器初始化
	LED_Config();					//led初始化
	OLED_Init();					//OLED屏初始化
	Key_Init();						//按键初始化
	relay_Init();					//继电器初始化

	OLED_Clear();
			
	while(1)
	{		
		//若需要其他定时时间在Delay.c与.h中添加
		if(timekey[0]>=timekey[1]) //滴答定时器10ms扫描一次按键
		{
			Menu_key_set();					 //获取按键数据
			timekey[0] = 0;					 //滴答定时器计数清零
		}
		
		if(timedi[0]>=timedi[1])	 //1S执行一次
		{
			timedi[0] = 0;
		}
		
		if(timeled[0]>=timeled[1]) //2S执行一次
		{
			GetData();							 //获取传感器数据
			timeled[0] = 0;
		}
		
		Alarm();
		
		//Key_Mune();								 //页面显示
		OLED_Update();						 //数据更新
		OLED_Clear();
		ShowData();
		}
}

void ShowData(void)
{
	//数据页面显示
}


void GetData(void)
{
	//读取数据
}

void Menu_key_set(void)									//按键扫描
{
	key1 = Get_Key_1();										//按键1调节页面
	key2 = Get_Key_2();										//按键2调节时间
	key3 = Get_Key_3();										//按键3调节时间
	
	if(key1 == 1)
	{
		i++;																//按键1按下页面更新
	}
	
	if(i >= 6)														//有几个页面 i >= j，j就改为多少
	{
		i = 0;															//页面浏览完重回初始数据页面
	}
}

void Alarm(void)
{
	//数据处理，判断数据是否异常，异常后执行的操
	
}
