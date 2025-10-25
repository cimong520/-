#include "stm32f10x.h"                  // Device header
#include "key.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "Delay.h"
#include "menu.h"
#include "../HardWare/relay.h"
#include "../HardWare/Serial.h"

// 全局变量定义
uint8_t mode_flag = 0;        // 模式标志
uint8_t key_flag = 0;        // 按键标志
uint8_t flag = 1;

uint8_t Oxygen_Lower = 80; //Ѫ����ֵ���
uint8_t Oxygen_Upper = 150;//Ѫ����ֵ���
uint8_t Heart_Lower = 80;
uint8_t Heart_Upper = 150;
float Temperature_Lower = 21;
float Temperature_Upper = 27;
int Humidity_Lower = 40;
int Humidity_Upper = 80;


//int menu1(void)
//{
//	while(1)
//	{
//		OLED_ShowString(0,0, "->              ",OLED_8X16);
//		OLED_ShowString(0,16,"Blood Oxygen Threshold",OLED_8X16);
//		OLED_ShowString(0,32,"Heart Rate Threshold",OLED_8X16);
//		OLED_ShowString(0,48,"Temperature Threshold",OLED_8X16);
//		
//		OLED_Update();
//		if(Get_Key_1()== 1)
//		{
//			flag--;
//			if(flag == 0 ){
//				flag = 4;
//			}
//		}
//		if(Get_Key_2()== 2)
//		{
//			flag++;
//			if(flag == 5 ){
//				flag = 1;
//			}
//		}
//		if(Get_Key_3()== 3)
//		{
//			OLED_Clear();
//			OLED_Update();
//			return flag;
//		}
//		switch(flag)
//		{
//			case 1:{
//				OLED_ShowString(0,0, "->              ",OLED_8X16);
//				OLED_ShowString(0,16,"Blood Oxygen Threshold",OLED_8X16);
//				OLED_ShowString(0,32,"Heart Rate Threshold",OLED_8X16);
//				OLED_ShowString(0,48,"Temperature Threshold",OLED_8X16);
//				OLED_ReverseArea(0, 0, 128, 16);
//				OLED_Update();
//				break;
//			}
//			case 2:{
//				OLED_ShowString(0,0, "->              ",OLED_8X16);
//				OLED_ShowString(0,16,"Blood Oxygen Threshold",OLED_8X16);
//				OLED_ShowString(0,32,"Heart Rate Threshold",OLED_8X16);
//				OLED_ShowString(0,48,"Temperature Threshold",OLED_8X16);
//				OLED_ReverseArea(0, 16, 128, 16);
//				OLED_Update();
//				break;
//			}
//			case 3:{
//				OLED_ShowString(0,0, "->              ",OLED_8X16);
//				OLED_ShowString(0,16,"Blood Oxygen Threshold",OLED_8X16);
//				OLED_ShowString(0,32,"Heart Rate Threshold",OLED_8X16);
//				OLED_ShowString(0,48,"Temperature Threshold",OLED_8X16);
//				OLED_ReverseArea(0, 32, 128, 16);
//				OLED_Update();
//				break;
//			}
//			case 4:{
//				OLED_ShowString(0,0, "->              ",OLED_8X16);
//				OLED_ShowString(0,16,"Blood Oxygen Threshold",OLED_8X16);
//				OLED_ShowString(0,32,"Heart Rate Threshold",OLED_8X16);
//				OLED_ShowString(0,48,"Temperature Threshold",OLED_8X16);
//				OLED_ReverseArea(0, 48, 128, 16);
//				OLED_Update();
//				break;
//			}
//				
//		}
//	}
//		
//}
//
//int menu1(void)
//{
//	uint8_t flag = 1;
//    // ��ʼ����ʾ
//    OLED_Clear();
//    OLED_ShowString(0,0, "->              ",OLED_8X16);
//    OLED_ShowString(0,16,"Blood Oxygen Threshold",OLED_8X16);
//    OLED_ShowString(0,32,"Heart Rate Threshold",OLED_8X16);
//    OLED_ShowString(0,48,"Temperature Threshold",OLED_8X16);
//    OLED_ReverseArea(0, (flag-1) * 16, 128, 16);  // ��ʼ����
//    OLED_Update();
//    
//    uint8_t last_flag = flag;
//    
//    while(1)
//    {
//        // ��������
//        if(Get_Key_1() == 1)
//        {
//            flag--;
//            if(flag == 0) flag = 4;
//        }
//        if(Get_Key_2() == 2)
//        {
//            flag++;
//            if(flag == 5) flag = 1;
//        }
//        if(Get_Key_3() == 3)
//        {
//            OLED_Clear();
//            OLED_Update();
//            return flag;
//        }
//        
//        // ֻ��ѡ��ı�ʱ������ʾ
//        if(flag != last_flag)
//        {
//            OLED_Clear();
//            OLED_ShowString(0,0, "->              ",OLED_8X16);
//            OLED_ShowString(0,16,"Blood Oxygen Threshold",OLED_8X16);
//            OLED_ShowString(0,32,"Heart Rate Threshold",OLED_8X16);
//            OLED_ShowString(0,48,"Temperature Threshold",OLED_8X16);
//            OLED_ReverseArea(0, (flag-1) * 16, 128, 16);
//            OLED_Update();
//            last_flag = flag;
//        }
//        
//        //Delay_ms(50);  // ��ֹ������������
//    }
//}

int menu1(void)
{
	uint8_t flag = 1;
	uint8_t menu_flag;
    // ��Xʼ����ʾ
    OLED_Clear();
    OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Blood Oxygen Threshold",OLED_8X16);
    OLED_ShowString(0,32,"Heart Rate Threshold",OLED_8X16);
    OLED_ShowString(0,48,"Temperature Threshold",OLED_8X16);
    OLED_ReverseArea(0, (flag-1) * 16, 128, 16);  // ��ʼ����
    OLED_Update();
    
    uint8_t last_flag = flag;
    
    while(1)
    {
        // ��������
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
        // ֻ��ѡ��ı�ʱ������ʾ
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
        
        //Delay_ms(50);  // ��ֹ������������
		
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
    uint8_t flag = 1;           // ��ǰѡ���ѡ�� (1-5)
    uint8_t menu_flag = 0;      // ȷ��ѡ���ѡ��
    uint8_t display_offset = 0; // ��ʾƫ����
    uint8_t last_flag = 0;      // �ϴε�flagֵ
    uint8_t last_offset = 255;  // �ϴε�ƫ����(��ʼ��Ϊ������ֵ)
    
    const uint8_t TOTAL_ITEMS = 5;    // ��ѡ����
    const uint8_t VISIBLE_ITEMS = 4;  // �ɼ�ѡ����
    
    // �˵�ѡ������
    const char* menu_items[5] = {
        "Temp Threshold",
        "Humi Threshold", 
        "Manual Mode",
        "Automatic Mode",
        "Exit Menu"
    };
    
    // ��ʼ��ʾ
    OLED_Clear();
    
    while(1)
    {
        // ��������
        if(Get_Key_2())  // ����
        {
            flag--;
            if(flag == 0) flag = TOTAL_ITEMS;  // ѭ�������һ��
        }
        
        if(Get_Key_3())  // ����
        {
            flag++;
            if(flag > TOTAL_ITEMS) flag = 1;   // ѭ������һ��
        }
        
        if(Get_Key_4())  // ȷ��ѡ��
        {
            menu_flag = flag;
            
        }
        
        // ������ʾƫ����
        if(flag <= VISIBLE_ITEMS)
        {
            // ѡ����ǰ4�����Ҫ����
            display_offset = 0;
        }
        else
        {
            // ѡ�񳬹�4���Ҫ����
            display_offset = flag - VISIBLE_ITEMS;
        }
        
        // ֻ��ѡ��ı��ƫ�Ƹı�ʱ������ʾ
        if(flag != last_flag || display_offset != last_offset)
        {
            OLED_Clear();
            
            // ��ʾ��ͷ
			if(flag <= VISIBLE_ITEMS){
				OLED_ShowString(0, (flag-1)*16, "->", OLED_8X16);
			}else{
				 OLED_ShowString(0, 3*16, "->", OLED_8X16);
			}
           
            
            // ��ʾ�˵���
            for(uint8_t i = 0; i < VISIBLE_ITEMS; i++)
            {
                uint8_t item_index = display_offset + i;
                if(item_index < TOTAL_ITEMS)
                {
                    OLED_ShowString(20, i * 16, (char*)menu_items[item_index], OLED_8X16);
                }
            }
            
            // �����ͷλ��
            uint8_t arrow_pos = (flag - 1 - display_offset) * 16;
            OLED_ReverseArea(0, arrow_pos, 128, 16);
            
            OLED_Update();
            
            last_flag = flag;
            last_offset = display_offset;
        }
        
        //Delay_ms(50);  // ��ֹ������������
		switch(menu_flag)
		{
			case 1:
				// �¶���ֵ����
				int result = Temperature_Threshold();
                if(result == 0) {
                    OLED_Clear();  // ���� menu2 ʱ�ػ�
                    last_flag = 0; // ǿ��ˢ����ʾ
                    menu_flag = 0; // ����ѡ��
                }
				break;
				
			case 2:
				// ʪ����ֵ����
				Humidity_Threshold();
                if(Humidity_Threshold() == 0) {
                    OLED_Clear();  // ���� menu2 ʱ�ػ�
                    last_flag = 0; // ǿ��ˢ����ʾ
                    menu_flag = 0; // ����ѡ��
                }
				break;
				
			case 3:
				mode_flag = 1;// �ֶ�ģʽ
				Serial_SendString("mode_flag = 1");
				break;
				
			case 4:
				mode_flag = 0; // �Զ�����
			    Serial_SendString("mode_flag = 0");
				break;
				
			case 5:
				// �˳��˵�
				return 0;
				
		}
		
    }
    
    // �����˵�ѡ��
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
		//��������
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
	OLED_ReverseArea(8*12,16,128,16);//����4������λ
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
		OLED_ReverseArea(8*12,16,128,16);//����4������λ
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
	OLED_ReverseArea(8*12,32,128,16);//����4������λ
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
		OLED_ReverseArea(8*12,32,128,16);//����4������λ
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
		//��������
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
	OLED_ReverseArea(8*12,16,128,16);//����4������λ
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
		OLED_ReverseArea(8*12,16,128,16);//����4������λ
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
	OLED_ReverseArea(8*12,32,128,16);//����4������λ
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
		OLED_ReverseArea(8*12,32,128,16);//����4������λ
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
		//��������
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
				Temperature_Low_set();// �¶��������
				if(Temperature_Low_set() == 0)
				{
					menu_flag = 0;
					lastflag = 0;
					OLED_Clear();
				}
				break;
				
			case 3:
				Temperature_Up_set();// �¶��������
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
	OLED_ReverseArea(8*12,16,128,16);//����4������λ
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
		OLED_ReverseArea(8*12,16,128,16);//����4������λ
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
	OLED_ReverseArea(8*12,32,128,16);//����4������λ
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
		OLED_ReverseArea(8*12,32,128,16);//����4������λ
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
		//��������
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
				Humidity_Low_set();// ʪ���������
				if(Humidity_Low_set() == 0)
				{
					menu_flag = 0;
					lastflag = 0;
					OLED_Clear();
				}
				break;
				
			case 3:
				Humidity_Up_set();// �¶��������
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

int Humidity_Low_set(void){
	//��ʼ��ʾ
	OLED_Clear();
	OLED_ShowString(0,0, "->              ",OLED_8X16);
    OLED_ShowString(0,16,"Humi_Limit:",OLED_8X16);
	OLED_ShowNum(8*12,16,Humidity_Lower,3,OLED_8X16);
    OLED_ShowString(0,32,"Humi_Upper:",OLED_8X16);
	OLED_ShowNum(8*12,32,Humidity_Upper,3,OLED_8X16);
	OLED_ReverseArea(8*12,16,128,16);//����4������λ
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
		OLED_ReverseArea(8*12,16,128,16);//����4������λ
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
	OLED_ReverseArea(8*12,32,128,16);//����4������λ
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
		OLED_ReverseArea(8*12,32,128,16);//����4������λ
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
