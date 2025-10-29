#include "stm32f10x.h"                  // Device header
#include "key.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "Delay.h"
#include "menu.h"
#include "main.h"
#include "../HardWare/relay.h"
#include "../HardWare/Serial.h"

// 全局变量定义
uint8_t mode_flag = 0;        // 模式标志
uint8_t key_flag = 0;        // 按键标志
uint8_t flag = 1;

// 医疗相关变量（未使用，可删除）
uint8_t Oxygen_Lower = 80;    // 血氧下限值
uint8_t Oxygen_Upper = 150;   // 血氧上限值
uint8_t Heart_Lower = 80;     // 心率下限值
uint8_t Heart_Upper = 150;    // 心率上限值

// 农业监控阈值变量
float Temperature_Lower = 15;  // 温度下限
float Temperature_Upper = 27;  // 温度上限
int Humidity_Lower = 40;       // 湿度下限
int Humidity_Upper = 80;       // 湿度上限

// 注意：data.Threshold的初始化应该在初始化函数中进行
// 例如在 System/Initialization.c 的 Data_Init() 函数中：
// data.Threshold.Soil_Low = 35;
// data.Threshold.Soil_Up = 40;
// data.Threshold.Light_Low = 15;
// data.Threshold.Light_Up = 20;

int menu1(void)
{
	uint8_t flag = 1;
	uint8_t menu_flag;
    // 初始化显示
    OLED_Clear();
    OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Blood Oxygen Threshold",OLED_8X16);
    OLED_ShowString(0,32,"Heart Rate Threshold",OLED_8X16);
    OLED_ShowString(0,48,"Temperature Threshold",OLED_8X16);
    OLED_ReverseArea(0, (flag-1) * 16, 128, 16);  // 初始高亮
    OLED_Update();
    
    uint8_t last_flag = flag;
    
    while(1)
    {
        // 按键处理
        if(Get_Key_2() == 2)
        {
            flag--;
            if(flag == 0) flag = 5;
        }
        if(Get_Key_3() == 3)
        {
            flag++;
            if(flag == 6) flag = 1;
        }
        if(Get_Key_4() == 4)
        {
            OLED_Clear();
            OLED_Update();
            menu_flag = flag;
        }
//        if(menu_flag == 1){
//			return 0;
//		}
        // 只在选项改变时更新显示
        if(flag != last_flag)
        {
            OLED_Clear();
            OLED_ShowString(0,0, "->              ",OLED_8X16);
            OLED_ShowString(0,16,"temp Threshold",OLED_8X16);
            OLED_ShowString(0,32,"humi Threshold",OLED_8X16);
            OLED_ShowString(0,48,"Manual Mode",OLED_8X16);
            OLED_ReverseArea(0, ((flag-1)%4) * 16, 128, 16);
            OLED_Update();
            last_flag = flag;
        }
        
        // Delay_ms(50);  // 防止按键抖动
		
		switch(menu_flag){
			case 1:{return 0;break;}
			case 2:{Blood_Oxygen_Threshold();}
			case 3:{}
			case 4:{}
		} 
    }
}

int menu2(void)
{
    uint8_t flag = 1;           // 当前选中选项 (1-5)
    uint8_t menu_flag = 0;      // 确认选中选项
    uint8_t display_offset = 0; // 显示偏移量
    uint8_t last_flag = 0;      // 上次的flag值
    uint8_t last_offset = 255;  // 上次的偏移量(初始化为无效值)
    
    const uint8_t TOTAL_ITEMS = 7;    // 总选项数
    const uint8_t VISIBLE_ITEMS = 4;  // 可见选项数
    
    // 菜单选项数组
    const char* menu_items[7] = {
        "Temp Threshold",
        "Humi Threshold", 
		"Soil Threshold",
		"Light Threshold",
        "Manual Mode",
        "Automatic Mode",
        "Exit Menu"
    };
    
    // 初始显示
    OLED_Clear();
    
    while(1)
    {
        // 按键处理
        if(Get_Key_2())  // 上移
        {
            flag--;
            if(flag == 0) flag = TOTAL_ITEMS;  // 循环到最后一项
        }
        
        if(Get_Key_3())  // 下移
        {
            flag++;
            if(flag > TOTAL_ITEMS) flag = 1;   // 循环到第一项
        }
        
        if(Get_Key_4())  // 确认选择
        {
            menu_flag = flag;
            
        }
        
        // 计算显示偏移量
        if(flag <= VISIBLE_ITEMS)
        {
            // ѡ下移ǰ4�下移Ҫ下移
            display_offset = 0;
        }
        else
        {
            // ѡ�񳬹�4���Ҫ下移
            display_offset = flag - VISIBLE_ITEMS;
        }
        
        // 只在选项改变或偏移改变时更新显示
        if(flag != last_flag || display_offset != last_offset)
        {
            OLED_Clear();
            
            // 显示箭头
			if(flag <= VISIBLE_ITEMS){
				OLED_ShowString(0, (flag-1)*16, "->", OLED_8X16);
			}else{
				 OLED_ShowString(0, 3*16, "->", OLED_8X16);
			}
           
            
            // 显示菜单项
            for(uint8_t i = 0; i < VISIBLE_ITEMS; i++)
            {
                uint8_t item_index = display_offset + i;
                if(item_index < TOTAL_ITEMS)
                {
                    OLED_ShowString(20, i * 16, (char*)menu_items[item_index], OLED_8X16);
                }
            }
            
            // 反转箭头位置
            uint8_t arrow_pos = (flag - 1 - display_offset) * 16;
            OLED_ReverseArea(0, arrow_pos, 128, 16);
            
            OLED_Update();
            
            last_flag = flag;
            last_offset = display_offset;
        }
        
        Delay_ms(50);  // ��ֹ按键处理下移
		switch(menu_flag)
		{
			case 1:
				// 温度阈值设置
				int result = Temperature_Threshold();
                if(result == 0) {
                    OLED_Clear();  // 下移 menu2 ʱ�ػ�
                    last_flag = 0; // 强制刷新显示
                    menu_flag = 0; // 重置选项
                }
				break;
				
			case 2:
				// 湿度阈值设置
				Humidity_Threshold();
                if(Humidity_Threshold() == 0) {
                    OLED_Clear();  // 下移 menu2 ʱ�ػ�
                    last_flag = 0; // 强制刷新显示
                    menu_flag = 0; // 重置选项
                }
				break;
				
			case 3:
				//土壤湿度阈值
				Soil_Threshold();
				if(Soil_Threshold () == 0) {
                    OLED_Clear();  // 下移 menu2 ʱ�ػ�
                    last_flag = 0; // 强制刷新显示
                    menu_flag = 0; // 重置选项
                }
				break;
				
			case 4:
				//光照强度阈值
				Light_Threshold();
				if(Light_Threshold() == 0) {
                    OLED_Clear();  // 下移 menu2 ʱ�ػ�
                    last_flag = 0; // 强制刷新显示
                    menu_flag = 0; // 重置选项
                }
				break;
			
			case 5:
				mode_flag = 1;// 手动模式
				Serial_SendString("mode_flag = 1");
				break;
			
			case 6:
				mode_flag = 0; // 自动控制
			    Serial_SendString("mode_flag = 0");
				break;
				
			case 7:
				// 退出菜单
				OLED_Clear();
				return 0;
				
		}
		
    }
    
    // 下移�˵�ѡ��
    OLED_Clear();
    OLED_Update();
	
    
}


int Blood_Oxygen_Threshold(void)
{
	
	uint8_t flag = 1;
	uint8_t lastflag = flag;
	uint8_t Blood_flag;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
	OLED_ShowNum(8*12,16,Oxygen_Lower,4,OLED_8X16);
    OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
	OLED_ShowNum(8*12,32,Oxygen_Upper,4,OLED_8X16);
	OLED_ReverseArea(0,(flag-1)*16,128,16);
	OLED_Update();
	while(1)
	{
		//按键处理
		if(Get_Key_1() == 1)
		{
			flag--;
			if(flag == 0)
			{
				flag = 3;
			}
		}
		if(Get_Key_2() == 2)
		{
			flag++;
			if(flag == 4)
			{
				flag = 1;
			}
		}
		if(Get_Key_3() == 3)
		{
			
			OLED_Clear();
			OLED_Update();
			Blood_flag = flag;
		}
		if(lastflag != flag){
			OLED_Clear();
			OLED_ShowString(0,0, "->              ",OLED_8X16);
			OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
			OLED_ShowNum(8*12,16,Oxygen_Lower,4,OLED_8X16);
			OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
			OLED_ShowNum(8*12,32,Oxygen_Upper,4,OLED_8X16);
			OLED_ReverseArea(0,(flag-1)*16,128,16);
			lastflag = flag;
			OLED_Update();
		}
		switch(Blood_flag ){
		
			case 1:{  return 0;  break;}
			case 2:{}
			case 3:{}
		}
		
	}
	
}
int Blood_Oxygen_Low_set(void){
	uint8_t flag = 1;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
	OLED_ShowNum(8*12,16,Oxygen_Lower,4,OLED_8X16);
    OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
	OLED_ShowNum(8*12,32,Oxygen_Upper,4,OLED_8X16);
	OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_1() == 1)
		{
			Oxygen_Lower--;
			if(Oxygen_Lower == 0)
			{
				Oxygen_Lower = 80;
			}
		}
		if(Get_Key_2() == 2)
		{
			Oxygen_Lower++;
			if(Oxygen_Lower == 81)
			{
				Oxygen_Lower = 80;
			}
		}
		if(Get_Key_3() == 3)
		{
			
			OLED_Clear();
			OLED_Update();
			return flag;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
		OLED_ShowNum(8*12,16,Oxygen_Lower,4,OLED_8X16);
		OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
		OLED_ShowNum(8*12,32,Oxygen_Upper,4,OLED_8X16);
		OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
		OLED_Update();
	}
}

int Blood_Oxygen_Up_set(void){
	uint8_t flag = 1;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
	OLED_ShowNum(8*12,16,Oxygen_Lower,4,OLED_8X16);
    OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
	OLED_ShowNum(8*12,32,Oxygen_Upper,4,OLED_8X16);
	OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_1() == 1)
		{
			Oxygen_Upper--;
			if(Oxygen_Upper == 89)
			{
				Oxygen_Upper = 150;
			}
		}
		if(Get_Key_2() == 2)
		{
			Oxygen_Upper++;
			if(Oxygen_Upper == 151)
			{
				Oxygen_Upper = 90;
			}
		}
		if(Get_Key_3() == 3)
		{
			
			OLED_Clear();
			OLED_Update();
			return flag;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
		OLED_ShowNum(8*12,16,Oxygen_Lower,4,OLED_8X16);
		OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
		OLED_ShowNum(8*12,32,Oxygen_Upper,4,OLED_8X16);
		OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
		OLED_Update();
	}
}

int Heart_Rate_Threshold(void){
	uint8_t flag = 1;
	uint8_t lastflag = flag;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Heart_Lower:",OLED_8X16);
	OLED_ShowNum(8*12,16,Heart_Lower,4,OLED_8X16);
    OLED_ShowString(0,32,"Heart_Upper:",OLED_8X16);
	OLED_ShowNum(8*12,32,Heart_Upper,4,OLED_8X16);
	OLED_ReverseArea(0,(flag-1)*16,128,16);
	OLED_Update();
	while(1)
	{
		//按键处理
		if(Get_Key_1() == 1)
		{
			flag--;
			if(flag == 0)
			{
				flag = 3;
			}
		}
		if(Get_Key_2() == 2)
		{
			flag++;
			if(flag == 4)
			{
				flag = 1;
			}
		}
		if(Get_Key_3() == 3)
		{
			
			OLED_Clear();
			OLED_Update();
			return flag;
		}
		if(lastflag != flag){
			OLED_Clear();
			OLED_ShowString(0,0, "->              ",OLED_8X16);
			OLED_ShowString(0,16,"Heart_Lower:",OLED_8X16);
			OLED_ShowNum(8*12,16,Heart_Lower,4,OLED_8X16);
			OLED_ShowString(0,32,"Heart_Upper:",OLED_8X16);
			OLED_ShowNum(8*12,32,Heart_Upper,4,OLED_8X16);
			OLED_ReverseArea(0,(flag-1)*16,128,16);
			lastflag = flag;
			OLED_Update();
		}
		
	}
}

int Heart_Rate_Low_set(void){
	uint8_t flag = 1;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
	OLED_ShowNum(8*12,16,Heart_Lower,4,OLED_8X16);
    OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
	OLED_ShowNum(8*12,32,Heart_Upper,4,OLED_8X16);
	OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_1() == 1)
		{
			Heart_Lower--;
			if(Heart_Lower == 0)
			{
				Heart_Lower = 80;
			}
		}
		if(Get_Key_2() == 2)
		{
			Heart_Lower++;
			if(Heart_Lower == 81)
			{
				Heart_Lower = 80;
			}
		}
		if(Get_Key_3() == 3)
		{
			
			OLED_Clear();
			OLED_Update();
			return flag;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
		OLED_ShowNum(8*12,16,Heart_Lower,4,OLED_8X16);
		OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
		OLED_ShowNum(8*12,32,Heart_Upper,4,OLED_8X16);
		OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
		OLED_Update();
	}
}

int Heart_Rate_Up_set(void){
	uint8_t flag = 1;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
	OLED_ShowNum(8*12,16,Heart_Lower,4,OLED_8X16);
    OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
	OLED_ShowNum(8*12,32,Heart_Upper,4,OLED_8X16);
	OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_1() == 1)
		{
			Heart_Upper--;
			if(Heart_Upper == 89)
			{
				Heart_Upper = 150;
			}
		}
		if(Get_Key_2() == 2)
		{
			Heart_Upper++;
			if(Heart_Upper == 151)
			{
				Heart_Upper = 90;
			}
		}
		if(Get_Key_3() == 3)
		{
			
			OLED_Clear();
			OLED_Update();
			return flag;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Lower Limit:",OLED_8X16);
		OLED_ShowNum(8*12,16,Heart_Lower,4,OLED_8X16);
		OLED_ShowString(0,32,"Upper Limit:",OLED_8X16);
		OLED_ShowNum(8*12,32,Heart_Upper,4,OLED_8X16);
		OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
		OLED_Update();
	}
}
int Temperature_Threshold(void){
	uint8_t flag = 1;
	uint8_t lastflag = flag;
	uint8_t menu_flag = 255;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Temp_Lower:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,Temperature_Lower,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Temp_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,Temperature_Upper,2,1,OLED_8X16);
	OLED_ReverseArea(0,(flag-1)*16,128,16);
	OLED_Update();
	while(1)
	{
		//按键处理
		if(Get_Key_2() == 2)
		{
			flag--;
			if(flag == 0)
			{
				flag = 3;
			}
		}
		if(Get_Key_3() == 3)
		{
			flag++;
			if(flag == 4)
			{
				flag = 1;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			menu_flag = flag;
		}
		if(lastflag != flag){
			OLED_Clear();
			OLED_ShowString(0,0, "->              ",OLED_8X16);
			OLED_ShowString(0,16,"Temp_Lower:",OLED_8X16);
			OLED_ShowFloatNum(8*12,16,Temperature_Lower,2,1,OLED_8X16);
			OLED_ShowString(0,32,"Temp_Upper:",OLED_8X16);
			OLED_ShowFloatNum(8*12,32,Temperature_Upper,2,1,OLED_8X16);
			OLED_ReverseArea(0,(flag-1)*16,128,16);
			lastflag = flag;
			OLED_Update();
		}
		switch(menu_flag)
		{
			case 1:
			    return 0;
				break;// �˳�
				
				
			case 2:
				Temperature_Low_set();// �¶按键处理
				if(Temperature_Low_set() == 0)
				{
					menu_flag = 0;
					lastflag = 0;
					OLED_Clear();
				}
				break;
				
			case 3:
				Temperature_Up_set();// �¶按键处理
				if(Temperature_Up_set() == 0)
				{
					menu_flag = 0;
					lastflag = 0;
					OLED_Clear();
				}
				break;
				
				
		}
		
	}
}
int Temperature_Low_set(void){
	uint8_t flag = 1;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Temp_Limit:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,Temperature_Lower,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Temp_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,Temperature_Upper,2,1,OLED_8X16);
	OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_2() == 2)
		{
			Temperature_Lower -= 0.5 ;
			if(Temperature_Lower == 19.5)
			{
				Temperature_Lower = 30;
			}
		}
		if(Get_Key_3() == 3)
		{
			Temperature_Lower += 0.5;
			if(Temperature_Lower == 30.5)
			{
				Temperature_Lower = 20;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			return 0;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Temp_Limit:",OLED_8X16);
		OLED_ShowFloatNum(8*12,16,Temperature_Lower,2,1,OLED_8X16);
		OLED_ShowString(0,32,"Temp_Upper:",OLED_8X16);
		OLED_ShowFloatNum(8*12,32,Temperature_Upper,2,1,OLED_8X16);
		OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
		OLED_Update();
	}
}

int Temperature_Up_set(void){
	uint8_t flag = 1;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Temp_Limit:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,Temperature_Lower,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Temp_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,Temperature_Upper,2,1,OLED_8X16);
	OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_2() == 2)
		{
			Temperature_Upper -= 0.5 ;
			if(Temperature_Upper == 24.5)
			{
				Temperature_Upper = 40;
			}
		}
		if(Get_Key_3() == 3)
		{
			Temperature_Upper += 0.5;
			if(Temperature_Upper == 40.5)
			{
				Temperature_Upper = 25;
			}
		}
		if(Get_Key_4() == 4)
		{
			return 0;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Temp_Limit:",OLED_8X16);
		OLED_ShowFloatNum(8*12,16,Temperature_Lower,2,1,OLED_8X16);
		OLED_ShowString(0,32,"Temp_Upper:",OLED_8X16);
		OLED_ShowFloatNum(8*12,32,Temperature_Upper,2,1,OLED_8X16);
		OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
		OLED_Update();
	}
}

int Humidity_Threshold(void){
	uint8_t flag = 1;
	uint8_t lastflag = flag;
	uint8_t menu_flag = 255;
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Huni_Lower:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,Humidity_Lower,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Humi_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,Humidity_Upper,2,1,OLED_8X16);
	OLED_ReverseArea(0,(flag-1)*16,128,16);
	OLED_Update();
	while(1)
	{
		//按键处理
		if(Get_Key_2() == 2)
		{
			flag--;
			if(flag == 0)
			{
				flag = 3;
			}
		}
		if(Get_Key_3() == 3)
		{
			flag++;
			if(flag == 4)
			{
				flag = 1;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			menu_flag = flag;
		}
		if(lastflag != flag){
			OLED_Clear();
			OLED_ShowString(0,0, "->              ",OLED_8X16);
			OLED_ShowString(0,16,"Humi_Lower:",OLED_8X16);
			OLED_ShowFloatNum(8*12,16,Humidity_Lower,2,1,OLED_8X16);
			OLED_ShowString(0,32,"Humi_Upper:",OLED_8X16);
			OLED_ShowFloatNum(8*12,32,Humidity_Upper,2,1,OLED_8X16);
			OLED_ReverseArea(0,(flag-1)*16,128,16);
			lastflag = flag;
			OLED_Update();
		}
		switch(menu_flag)
		{
			case 1:
			    return 0;
				break;// �˳�
				
				
			case 2:
				Humidity_Low_set();// ʪ按键处理�
				if(Humidity_Low_set() == 0)
				{
					menu_flag = 0;
					lastflag = 0;
					OLED_Clear();
				}
				break;
				
			case 3:
				Humidity_Up_set();// �¶按键处理
				if(Humidity_Up_set() == 0)
				{
					menu_flag = 0;
					lastflag = 0;
					OLED_Clear();
				}
				break;				
		}
		
	}
}

int Humidity_Low_set(void){
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Humi_Limit:",OLED_8X16);
	OLED_ShowNum(8*12,16,Humidity_Lower,3,OLED_8X16);
    OLED_ShowString(0,32,"Humi_Upper:",OLED_8X16);
	OLED_ShowNum(8*12,32,Humidity_Upper,3,OLED_8X16);
	OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_2() == 2)
		{
			Humidity_Lower -= 1;
			if(Humidity_Lower == 29)
			{
				Humidity_Lower = 60;
			}
		}
		if(Get_Key_3() == 3)
		{
			Humidity_Lower += 1;
			if(Humidity_Lower == 61)
			{
				Humidity_Lower = 30;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			return 0;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Humi_Limit:",OLED_8X16);
		OLED_ShowNum(8*12,16,Humidity_Lower,3,OLED_8X16);
		OLED_ShowString(0,32,"Humi_Upper:",OLED_8X16);
		OLED_ShowNum(8*12,32,Humidity_Upper,3,OLED_8X16);
		OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
		OLED_Update();
	}
}

int Humidity_Up_set(void){
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Humi_Limit:",OLED_8X16);
	OLED_ShowNum(8*12,16,Humidity_Lower,3,OLED_8X16);
    OLED_ShowString(0,32,"Humi_Upper:",OLED_8X16);
	OLED_ShowNum(8*12,32,Humidity_Upper,3,OLED_8X16);
	OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_2() == 2)
		{
			Humidity_Upper -= 1;
			if(Humidity_Upper == 29)
			{
				Humidity_Upper = 120;
			}
		}
		if(Get_Key_3() == 3)
		{
			Humidity_Upper += 1;
			if(Humidity_Upper == 121)
			{
				Humidity_Upper = 30;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			return 0;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Humi_Limit:",OLED_8X16);
		OLED_ShowNum(8*12,16,Humidity_Lower,3,OLED_8X16);
		OLED_ShowString(0,32,"Humi_Upper:",OLED_8X16);
		OLED_ShowNum(8*12,32,Humidity_Upper,3,OLED_8X16);
		OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
		OLED_Update();
	}
}

int Soil_Threshold (void)
{
	uint8_t flag = 1;
	uint8_t lastflag = flag;
	uint8_t menu_flag = 255;
	//初始显示
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Soil_Lower:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,data.Threshold.Soil_Low,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Soil_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,data.Threshold.Soil_Up,2,1,OLED_8X16);
	OLED_ReverseArea(0,(flag-1)*16,128,16);
	OLED_Update();
	while(1)
	{
		//按键处理
		if(Get_Key_2() == 2)
		{
			flag--;
			if(flag == 0)
			{
				flag = 3;
			}
		}
		if(Get_Key_3() == 3)
		{
			flag++;
			if(flag == 4)
			{
				flag = 1;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			menu_flag = flag;
		}
		if(lastflag != flag){
			OLED_Clear();
			OLED_ShowString(0,0, "->              ",OLED_8X16);
			OLED_ShowString(0,16,"Soil_Lower:",OLED_8X16);
			OLED_ShowFloatNum(8*12,16,data.Threshold.Soil_Low,2,1,OLED_8X16);
			OLED_ShowString(0,32,"Soil_Upper:",OLED_8X16);
			OLED_ShowFloatNum(8*12,32,data.Threshold.Soil_Up,2,1,OLED_8X16);
			OLED_ReverseArea(0,(flag-1)*16,128,16);
			lastflag = flag;
			OLED_Update();
		}
		switch(menu_flag)
		{
			case 1:
			    return 0;
				break;// �˳�
				
				
			case 2:
				Soil_Low_set();// ʪ按键处理�
				if(Soil_Low_set() == 0)
				{
					OLED_Clear();
					menu_flag = 0;
					lastflag = 0;	
				}
				break;
				
			case 3:
				Soil_Up_set();// �¶按键处理
				if(Soil_Up_set() == 0)
				{
					OLED_Clear();
					menu_flag = 0;
					lastflag = 0;
				}
				break;				
		}
		
	}
}

int Soil_Low_set(void)
{
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Soil_Lower:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,data.Threshold.Soil_Low,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Soil_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,data.Threshold.Soil_Up,2,1,OLED_8X16);
	OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_2() == 2)
		{
			data.Threshold.Soil_Low -= 5;
			if(data.Threshold.Soil_Low == 0)
			{
				data.Threshold.Soil_Low = 50;
			}
		}
		if(Get_Key_3() == 3)
		{
			data.Threshold.Soil_Low += 5;
			if(data.Threshold.Soil_Low == 105)
			{
				data.Threshold.Soil_Low = 30;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			return 0;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Soil_Lower:",OLED_8X16);
		OLED_ShowFloatNum(8*12,16,data.Threshold.Soil_Low,2,1,OLED_8X16);
		OLED_ShowString(0,32,"Soil_Upper:",OLED_8X16);
		OLED_ShowFloatNum(8*12,32,data.Threshold.Soil_Up,2,1,OLED_8X16);
		OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
		OLED_Update();
	}
}
int Soil_Up_set(void)
{
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Soil_Lower:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,data.Threshold.Soil_Low,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Soil_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,data.Threshold.Soil_Up,2,1,OLED_8X16);
	OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_2() == 2)
		{
			data.Threshold.Soil_Up -= 5;
			if(data.Threshold.Soil_Up == 0)
			{
				data.Threshold.Soil_Up = 50;
			}
		}
		if(Get_Key_3() == 3)
		{
			data.Threshold.Soil_Up += 5;
			if(data.Threshold.Soil_Up == 105)
			{
				data.Threshold.Soil_Up = 50;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			return 0;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Soil_Lower:",OLED_8X16);
		OLED_ShowFloatNum(8*12,16,data.Threshold.Soil_Low,2,1,OLED_8X16);
		OLED_ShowString(0,32,"Soil_Upper:",OLED_8X16);
		OLED_ShowFloatNum(8*12,32,data.Threshold.Soil_Up,2,1,OLED_8X16);
		OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
		OLED_Update();
	}
}

int Light_Threshold (void){
	uint8_t flag = 1;
	uint8_t lastflag = flag;
	uint8_t menu_flag = 255;
	//初始显示
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Light_Lower:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,data.Threshold.Light_Low,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Light_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,data.Threshold.Light_Up,2,1,OLED_8X16);
	OLED_ReverseArea(0,(flag-1)*16,128,16);
	OLED_Update();
	while(1)
	{
		//按键处理
		if(Get_Key_2() == 2)
		{
			flag--;
			if(flag == 0)
			{
				flag = 3;
			}
		}
		if(Get_Key_3() == 3)
		{
			flag++;
			if(flag == 4)
			{
				flag = 1;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			menu_flag = flag;
		}
		if(lastflag != flag){
			OLED_Clear();
			OLED_ShowString(0,0, "->              ",OLED_8X16);
			OLED_ShowString(0,16,"Light_Lower:",OLED_8X16);
			OLED_ShowFloatNum(8*12,16,data.Threshold.Light_Low,2,1,OLED_8X16);
			OLED_ShowString(0,32,"Light_Upper:",OLED_8X16);
			OLED_ShowFloatNum(8*12,32,data.Threshold.Light_Up,2,1,OLED_8X16);
			OLED_ReverseArea(0,(flag-1)*16,128,16);
			lastflag = flag;
			OLED_Update();
		}
		switch(menu_flag)
		{
			case 1:
			    return 0;
				break;// �˳�
				
				
			case 2:
				Light_Low_set();// ʪ按键处理�
				if(Light_Low_set() == 0)
				{
					OLED_Clear();
					menu_flag = 0;
					lastflag = 0;	
				}
				break;
				
			case 3:
				Light_Up_set();// �¶按键处理
				if(Light_Up_set() == 0)
				{
					OLED_Clear();
					menu_flag = 0;
					lastflag = 0;
				}
				break;				
		}
	}
}
int Light_Low_set(void){
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Light_Lower:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,data.Threshold.Light_Low,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Light_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,data.Threshold.Light_Up,2,1,OLED_8X16);
	OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_2() == 2)
		{
			data.Threshold.Light_Low -= 1;
			if(data.Threshold.Light_Low == 0)
			{
				data.Threshold.Light_Low = 30;
			}
		}
		if(Get_Key_3() == 3)
		{
			data.Threshold.Light_Low += 5;
			if(data.Threshold.Light_Low == 85)
			{
				data.Threshold.Light_Low = 20;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			return 0;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Light_Lower:",OLED_8X16);
		OLED_ShowFloatNum(8*12,16,data.Threshold.Light_Low,2,1,OLED_8X16);
		OLED_ShowString(0,32,"Light_Upper:",OLED_8X16);
		OLED_ShowFloatNum(8*12,32,data.Threshold.Light_Up,2,1,OLED_8X16);
		OLED_ReverseArea(8*12,16,128,16);//下移4下移��λ
		OLED_Update();
	}
}
int Light_Up_set(void){
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Light_Lower:",OLED_8X16);
	OLED_ShowFloatNum(8*12,16,data.Threshold.Light_Low,2,1,OLED_8X16);
    OLED_ShowString(0,32,"Light_Upper:",OLED_8X16);
	OLED_ShowFloatNum(8*12,32,data.Threshold.Light_Up,2,1,OLED_8X16);
	OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
	OLED_Update();
	while(1)
	{
		if(Get_Key_2() == 2)
		{
			data.Threshold.Light_Up -= 5;
			if(data.Threshold.Light_Up == 0)
			{
				data.Threshold.Light_Up = 30;
			}
		}
		if(Get_Key_3() == 3)
		{
			data.Threshold.Light_Up += 5;
			if(data.Threshold.Light_Up == 85)
			{
				data.Threshold.Light_Up = 30;
			}
		}
		if(Get_Key_4() == 4)
		{
			
			return 0;
		}
		OLED_Clear();
		OLED_ShowString(0,0, "->              ",OLED_8X16);
		OLED_ShowString(0,16,"Light_Lower:",OLED_8X16);
		OLED_ShowFloatNum(8*12,16,data.Threshold.Light_Low,2,1,OLED_8X16);
		OLED_ShowString(0,32,"Light_Upper:",OLED_8X16);
		OLED_ShowFloatNum(8*12,32,data.Threshold.Light_Up,2,1,OLED_8X16);
		OLED_ReverseArea(8*12,32,128,16);//下移4下移��λ
		OLED_Update();
	}
}

void Manual_Mode( void ){
	if(mode_flag == 1)
	{
		if(Get_Key_3() == 3){
			key_flag++;
			if((key_flag % 2) == 1)
			{
				relay_ON();
			}else{
				relay_OFF();
			}
		}
	}
}
