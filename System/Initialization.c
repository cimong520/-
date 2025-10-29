#include "./Initialization.h"
#include "./Initialization_Data.h"

void Data_Init(void){
	
	// 初始化变量（重要！）
    data.Variable.Temp = 0.0;
    data.Variable.Humi = 0.0;
    data.Variable.Soil = 0.0;   // ← 添加这个
    data.Variable.Light = 0.0;  // ← 添加这个
	
	// 初始化阈值
	data.Threshold.Temp = 27.0;      // 温度上限
	data.Threshold.Humi = 80.0;      // 湿度上限
	data.Threshold.Soil_Low = 28.0;  // 土壤湿度下限
	data.Threshold.Soil_Up = 40.0;   // 土壤湿度上限
	data.Threshold.Light_Low = 5.0; // 光照下限
	data.Threshold.Light_Up = 20.0;  // 光照上限
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
	AD_Init();	                  //土壤湿度ADC通道初始化
	Init_BH1750();                //光照传感器初始化
	Buzzer_Init();                //蜂鸣器初始化
	// Check_the_network(); // 暂时注释掉，避免发送干扰命令
	Feng_Init();
//	模块初始化调用
// 补充Initialization()中没有的初始化
	while(DHT11_Init())
	{
		OLED_ShowString( 0, 0, "error", OLED_8X16);
		Delay_ms(1000);
	}      // DHT11温湿度传感器初始化
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
/*------------------变量声明---------------------*/
dt data;
char c ='7';
uint8_t buff[512]={0};
//DHT11_Data_TypeDef DHT11_Data = {0}; // 添加DHT11数据结构体变量
uint8_t connect_flag;			// 0：未连接  1：连接成功  2；服务器选择成功

// ESP8266状态信息全局变量

char saved_mode[30] = "未知";
char saved_status[20] = "未连接";

//	模块结构体声明
uint8_t temp, humi;
uint8_t key_num;
//	模块结构体声明
/*---------------------------------------*/

// 任务函数实现
/**
 * @brief 数据获取逻辑
 * 在这里调用各模块的数据获取函数
 */
void GetData(void){
    // 数据获取逻辑
}

// 处理云端控制指令
void Process_Cloud_Control(void){
    static uint16_t last_motor_flag = 0;
    static uint8_t last_led = 0;
    
    // 检查继电器控制变化
    if(data.flag.motor_flag != last_motor_flag) {
        Serial_SendString("云端控制: 继电器状态变更为 ");
        Serial_SendNumber(data.flag.motor_flag, 1);
        Serial_SendString("\r\n");
        
        if(data.flag.motor_flag > 0) {
            relay_ON();
            Serial_SendString("继电器已开启\r\n");
        } else {
            relay_OFF();
            Serial_SendString("继电器已关闭\r\n");
        }
        last_motor_flag = data.flag.motor_flag;
    }
    
    // 检查LED控制变化
    if(data.flag.led != last_led) {
        Serial_SendString("云端控制: LED状态变更为 ");
        Serial_SendNumber(data.flag.led, 1);
        Serial_SendString("\r\n");
        
        if(data.flag.led > 0) {
            // LED_ON();  // 如果有LED控制函数
            Serial_SendString("LED已开启\r\n");
        } else {
            // LED_OFF();
            Serial_SendString("LED已关闭\r\n");
        }
        last_led = data.flag.led;
    }
}

// 按键扫描逻辑
void Menu_key_set(void)									//按键扫描
{
	if (wifi.rxover == 1) {
		
		Extract_WiFi_Info();
		DataAnylize();  // 处理ESP8266数据（包括云端控制指令）
	}
//	data.flag.key1 = Get_Key_1();										
//	data.flag.key2 = Get_Key_2();										
//	data.flag.key3 = Get_Key_3();										
//	data.flag.key4 = Get_Key_4();	
//	if(data.flag.key1){
//		WIFI_FORGET_WIFI();
//	}
//	if(data.flag.key2){
//		data.flag.z=2;
//	}
//	if(data.flag.key3){
//		data.flag.z=3;
//	}
//	if(data.flag.key4){
//		data.flag.z=4;
//	}
}

// 报警处理逻辑
void Alarm(void)
{
	// 报警逻辑（蜂鸣器）
	if((data.Variable.Temp<Temperature_Lower)||(data.Variable.Temp > Temperature_Upper)||
		(data.Variable.Humi<Humidity_Lower)||(data.Variable.Humi>Humidity_Upper)||
		(data.Variable.Soil<data.Threshold.Soil_Low)||(data.Variable.Soil>data.Threshold.Soil_Up)||
		(data.Variable.Light<data.Threshold.Light_Low)||(data.Variable.Light>data.Threshold.Light_Up))
	{
		Buzzer_ON();
	}else{
		Buzzer_OFF();
	}
	
	// 继电器自动控制逻辑
	// 只有在自动模式(mode_flag==0)且没有云端控制时才执行
	if(mode_flag == 0)
	{
		// 只有当Soil在合理范围内才控制
        if(data.Variable.Soil >= 1.0 && data.Variable.Soil <= 100.0) {
            if(data.Variable.Soil < data.Threshold.Soil_Low) {
                relay_ON();
				data.flag.motor_flag = 1;  // 同步更新标志位
            }
            else if(data.Variable.Soil > data.Threshold.Soil_Up) {  // 修改：应该是大于上限才关闭
                relay_OFF();
				data.flag.motor_flag = 0;  // 同步更新标志位
            }
        }
	}
	// 如果是手动模式(mode_flag==1)，不在这里控制继电器
	// 继电器由云端控制或按键控制
}

void IWDGADD(void){
	MyRTC_ReadTime();
	data.flag.y = data.flag.x;
	data.flag.x=0;

	IWDG_ReloadCounter();

}

void WIFI_Init(void){
	if(connect_flag == 2){			//如果不需要时间可以将这个判断和内容移除掉
		WIFI_GET_TIME();
	}
	else if(connect_flag == 1){
		Connect_tothe_cloud(CMD_CONNECT_BAFA);	//连接到什么云端
	}
	else if(!connect_flag){
		Check_the_network();
	}
}

void upload_data(void){
	if(connect_flag == 2){			// 确保已连接到巴法云
		memset(buff, 0, 512);
		// 构造巴法云数据格式：CMD:SEND_DATA,{"Temp":25.5,"Humi":60.2,"Status":"Normal"}
		sprintf((char*)buff,
			"%s,{\"Temp\":%.1f,\"Humi\":%.1f,\"Soil\":%.1f,\"Light\":%.1f,\"Soil_Low\":%.1f,\"Wind\":%.1f,\"Status\":\"%s\",\"Relay\":\"%s\",}",
			CMD_SEND_DATA,
			data.Variable.Temp,
			data.Variable.Humi,
			data.Variable.Soil,
			data.Variable.Light,
			data.Threshold.Soil_Low,
		    data.Variable.Wind,
			(data.Variable.Temp > 30.0) ? "High" : "Normal",
			(data.flag.motor_flag) ? "ON" : "OFF"
		);
		WifiSendStr((char*)buff);
		
		// 通过串口输出调试信息
		Serial_SendString("Uploading to BaFa Cloud: ");
		Serial_SendString((char*)buff);
		Serial_SendString("\r\n");
	} else {
		// 输出连接状态调试信息
		if(connect_flag == 1) {
			Serial_SendString("错误: WiFi已连接，但巴法云未连接\r\n");
			Serial_SendString("请先按按键1连接巴法云\r\n");
		} else {
			Serial_SendString("错误: WiFi未连接，无法上传数据\r\n");
		}
		Serial_SendString("当前连接状态: ");
		Serial_SendNumber(connect_flag, 1);
		Serial_SendString(" (需要状态2才能上传)\r\n");
	}
}

//温度显示图像
// 简单的温度曲线函数
void SimpleDrawTempCurve(void)
{
    static float last_temps[64];  // 存储64个温度点
    static uint8_t temp_pos = 0;
    
    // 添加新温度数据
    last_temps[temp_pos] = (float)temp;
    temp_pos = (temp_pos + 1) % 64;
    
    // 清除曲线区域
    OLED_ClearArea(0, 20, 128, 40);
    
    // 绘制简单曲线
    for(uint8_t i = 1; i < 64; i++) {
        uint8_t x1 = i * 2;
        uint8_t x2 = (i + 1) * 2;
        uint8_t y1 = 60 - (uint8_t)(last_temps[i] / 2);      // 简单缩放
        uint8_t y2 = 60 - (uint8_t)(last_temps[i+1] / 2);
        
        if(x2 < 128 && y1 < 64 && y2 < 64) {
            OLED_DrawLine(x1, y1, x2, y2);
        }
    }
}

//温湿度和土壤湿度显示函数
void Update_TempHuim_Display(void)
{
	DHT11_Read_Data(&temp, &humi);
    static float Humi_Soil, Light_illunation, Wind_Speed;
	Humi_Soil = Get_SoilHumi(ADC_Channel_7, 3); 
    Light_illunation = read_BH1750();
	Wind_Speed = Get_FengSpeed(ADC_Channel_5, 10);  // 读取风速数据
	
     // 更新到全局数据结构
    data.Variable.Temp = (float)temp;
    data.Variable.Humi = (float)humi;
	data.Variable.Soil = (float) Humi_Soil;
	data.Variable.Light = (float) Light_illunation;
	data.Variable.Wind = (float) Wind_Speed;
	
	// 只清除温湿度显示区域
	OLED_ClearArea(0, 0, 128, 16);
	OLED_ShowString(6*0, 0, "tep", OLED_6X8);
	OLED_ShowString(6*7, 0, "hui", OLED_6X8);
	OLED_ShowString(6*13, 0, "Soi", OLED_6X8);
	OLED_ShowString(0, 8*1, "Light", OLED_6X8);
	OLED_ShowString(6*9, 8*1, "Wind", OLED_6X8);  // 添加风速标签
	OLED_ShowNum(6*4, 0, temp, 2, OLED_6X8);
	OLED_ShowNum(6*11, 0, humi, 2, OLED_6X8);
	OLED_ShowFloatNum(6*17, 0, Humi_Soil, 2, 1, OLED_6X8);  // 显示浮点数
	OLED_ShowNum(6*6, 8*1, Light_illunation, 2, OLED_6X8);
	OLED_ShowFloatNum(6*14, 8*1, Wind_Speed, 2, 2, OLED_6X8);  // 显示风速
    SimpleDrawTempCurve();
	// 更新显示
	OLED_Update();
}

//ESP8266测试
void Test_ESP8266(void)
{
    static uint32_t last_status_time = 0;
    uint32_t current_time = Task_GetSystemTime();
    
    // 每10秒输出一次状态信息
    if(current_time - last_status_time >= 10000) {
        Serial_SendString("\r\n=== 系统状态信息 ===\r\n");
        Serial_SendString("WiFi: ");
        Serial_SendString(saved_wifi_name);
        Serial_SendString(" (");
        Serial_SendString(saved_ip);
        Serial_SendString(")\r\n");
        
        Serial_SendString("巴法云: ");
        if(connect_flag == 2) {
            Serial_SendString("云端已连接");
        } else if(connect_flag == 1) {
            Serial_SendString("WiFi已连接，未连接云端");
        } else {
            Serial_SendString("未连接");
        }
        Serial_SendString("\r\n");
        Serial_SendString("温度: ");
        Serial_SendNumber((uint32_t)data.Variable.Temp, 2);
        Serial_SendString("°C  湿度: ");
        Serial_SendNumber((uint32_t)data.Variable.Humi, 2);
        Serial_SendString("%\r\n");
        Serial_SendString("connect_flag: ");
        Serial_SendNumber(connect_flag, 1);
        Serial_SendString("\r\n");
        Serial_SendString("按键1=连接巴法云  按键2=更新WiFi状态  按键4=上传数据\r\n");
        Serial_SendString("===================\r\n\r\n");
        last_status_time = current_time;
    }
}

// 任务ID存储（全局变量）
TaskID keyTaskID, dataTaskID, alarmTaskID, showTaskID, IWDG_ID, wifiID, uploadID, testTaskID ,TempHuimID ;
/**
 * @brief 任务初始化
 * 
 * 添加系统需要的所有任务并保存任务ID
 */
void Task_Initialization(void){
	keyTaskID = Task_Add(Menu_key_set, 10, PRIORITY_CRITICAL, "KeyScan");
    dataTaskID = Task_Add(GetData, 50, PRIORITY_NORMAL, "GetData");
    alarmTaskID = Task_Add(Alarm, 100, PRIORITY_HIGH, "Alarm");
   // showTaskID = Task_Add(ShowData, 0, PRIORITY_NORMAL, "ShowData");
	IWDG_ID = Task_Add(IWDGADD, 1000, PRIORITY_HIGH, "IWDG_ID");
	// 恢复自动化任务
	wifiID = Task_Add(WIFI_Init, 5000, PRIORITY_NORMAL, "WIFI_Init");        // 每5秒检查WiFi和云端连接状态
	uploadID = Task_Add(upload_data, 30000, PRIORITY_NORMAL, "uploadID");    // 每30秒自动上传数据
	
	// 添加云端控制处理任务
	TaskID controlTaskID = Task_Add(Process_Cloud_Control, 100, PRIORITY_HIGH, "CloudControl"); // 每100ms检查控制指令
	
	// 添加ESP8266监控任务
	testTaskID = Task_Add(Test_ESP8266, 10000, PRIORITY_LOW, "ESP8266Monitor"); // 每10秒输出状态信息
	
	//温湿度显示
	TempHuimID = Task_Add(Update_TempHuim_Display, 2000, PRIORITY_NORMAL, "showTempHuim"); 
	
}
