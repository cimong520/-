#include "stm32f10x.h"		//STM32头文件
#include "Delay.h"				//延时函数头文件
#include "math.h"
#include "oled.h"					//OLED屏头文件
#include "key.h"					//按键头文件
#include "LED.h"					//LED灯头文件
#include "dht11.h"

uint16_t i; 			//i页面标志位
uint8_t key1=0,key2=0,key3=0; //按键标志位

float Temp1=0,Temp2=0;
float Humi1=0,Humi2=0;
DHT11_Data_TypeDef DHT11_Data;

void ShowData(void);    //数据页面
void GetData(void);			//获取数据
void Alarm(void);				//自动控制函数
void Menu_key_set(void);//按键扫描
void Key_Mune(void);		//按键切换页面操作

int main(void)
{	
	SystemInit();  
	Delay_Init();					//滴答定时器初始化
	OLED_Init();					//OLED屏初始化
	Key_Init();						//按键初始化
	DHT11_Init();	
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
		
		// 显示带中文的传感器信息
		OLED_Printf(0, 0, OLED_8X16, "温度:%.3f℃ \n湿度:%.2f%%", Temp1, Humi1); 
		
		Key_Mune();								 //页面显示
		}
}

void ShowData(void)
{
	//数据页面显示
}

void GetData(void)
{
	//读取数据
	data.Variable.Humi=DHT11_Data.humi_int+DHT11_Data.humi_deci/10.0;
	data.Variable.Temp=DHT11_Data.temp_int+DHT11_Data.temp_deci/10.0;
	while( ! DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS);
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
	
	if(i >= 2)														//有几个页面 i >= j，j就改为多少
	{
		i = 0;															//页面浏览完重回初始数据页面
	}
}

void Key_Mune(void)											//按键页面切换
{
	if(i == 0)														//初始数据页面
	{
		ShowData();
	}
	else if(i == 1)												
	{
		
	}
}

void Alarm(void)
{
	//数据处理，判断数据是否异常，异常后执行的操作
}
