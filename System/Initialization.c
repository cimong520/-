#include "./Initialization.h"
#include "./Initialization_Data.h"

void Data_Init(void){
	data.flag.mode = 1;
//	变量初始化
}

/**
 * @brief 系统初始化
 * 
 * 1. 硬件初始化
 * 2. 任务系统初始化
 * 3. 添加任务
 * @brief 模块初始化总函数
 * 在这里调用所有已添加模块的初始化函数
 */
void Initialization(void){
	SystemInit();  
	Delay_Init();					//滴答定时器初始化
	OLED_Init();					//OLED屏初始化
	LED_Config();
	Key_Init();						//按键初始化	
	MyRTC_Init();
	ESP8266_Config();
	// Check_the_network(); // 暂时注释掉，避免发送干扰命令
//	模块初始化调用
// 补充Initialization()中没有的初始化
	DHT11_Init();      // DHT11温湿度传感器初始化
	Buzzer_Init();     // 蜂鸣器初始化
	relay_Init();      // 继电器初始化
	Serial_Init();     // 串口1初始化（调试用）
//	模块初始化调用结束
	
	Data_Init();
	Task_Init();
	Task_Initialization();
	
	/*IWDG初始化*/
//	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//独立看门狗写使能
//	IWDG_SetPrescaler(IWDG_Prescaler_32);			//设置预分频为16
//	IWDG_SetReload(2499);							//设置重装值为2499，独立看门狗的超时时间为1000ms
//	IWDG_ReloadCounter();							//重装计数器，喂狗
//	IWDG_Enable();									//独立看门狗使能
}

 void LED_Config(void)
 {
	 RCC_APB2PeriphClockCmd(LED_GPIO_Clock,ENABLE);
	 
	GPIO_InitTypeDef GPIO_InitStructure;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Pin = LED_PIN;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 
	 GPIO_Init(LED_GPIO,&GPIO_InitStructure);
	 LED_OFF();
 }
 
 
void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK | KEY3_GPIO_CLK | KEY4_GPIO_CLK, ENABLE);
    
		GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
    GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN;
    GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);
}
