#include "../System/Initialization.h"
#include "../HardWare/OLED.h"

#include "../HardWare/key.h"
#include "../HardWare/esp8266.h"
#include "./main.h"
#include "./menu.h"
#include <string.h>



uint8_t main_mode_flag  = 0, main_key_flag = 0 ; 



/**
 * @brief 数据显示逻辑
 * 在这里显示各模块的数据
 */
//void ShowData(void){
//    OLED_Clear();
//    if(connect_flag == 2){
//		OLED_ShowImage(0,0,16,16,wifi_int);
//		OLED_ShowImage(16,0,16,16,server_int);
//	}else if(connect_flag == 1){
//		OLED_ShowImage(0,0,16,16,wifi_int);
//		OLED_ShowImage(16,0,16,16,server_out);
//	}
//	else {
//		OLED_ShowImage(0,0,16,16,wifi_out);
//		OLED_ShowImage(16,0,16,16,server_out);
//	}
//	OLED_Printf(32,0,8,"   %02d/%02d/%02d\n    %02d:%02d:%02d\n",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2],MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
//	
//    // 默认显示系统信息
//    OLED_Printf(0, 16, 16, "系统就绪你好新\n%d\n%d",data.flag.y,data.flag.z);
//    OLED_Update();
//}
//模式选择
void Mode(void) {
    if(Get_Key_3() == 3) {
        if(main_mode_flag == 0) {
            main_mode_flag = 1;  // 进入手动模式
        } else {
            // 在手动模式下，按键3控制继电器
            main_key_flag++;
            if((main_key_flag % 2) == 1) {
                relay_ON();
            } else {
                relay_OFF();
            }
        }
    }
}


// 手动连接巴法云的函数
void Connect_To_BaFa_Cloud(void)
{
    static uint32_t last_connect_time = 0;
    uint32_t current_time = Task_GetSystemTime();
    
    // 防止重复连接（5秒内只能连接一次）
    if(current_time - last_connect_time < 5000) {
        Serial_SendString("请等待5秒后再尝试连接...\r\n");
        return;
    }
    
    // 检查是否已经连接到云端
    if(connect_flag == 2) {
        Serial_SendString("巴法云已连接，无需重复连接\r\n");
        return;
    }
    
    // 检查WiFi连接状态 - 如果有IP地址就认为已连接
    if(connect_flag >= 1 || strstr(saved_status, "已连接") || strstr(saved_ip, "172.168")) {
        Serial_SendString("=== 开始连接巴法云 ===\r\n");
        Serial_SendString("密钥: 51d78bea923947ebbeace0075d96cc21\r\n");
        Serial_SendString("上传主题: tang1\r\n");
        Serial_SendString("控制主题: tang2\r\n");
        Serial_SendString("发送连接指令...\r\n");
        
        Connect_tothe_cloud(CMD_CONNECT_BAFA);
        last_connect_time = current_time;
        
        Serial_SendString("巴法云连接指令已发送，等待响应...\r\n");
        Serial_SendString("===================\r\n");
    } else {
        Serial_SendString("错误: WiFi未连接，无法连接巴法云\r\n");
        Serial_SendString("当前连接状态: ");
        Serial_SendString(saved_status);
        Serial_SendString("\r\n");
    }
}



int main(void)
{	
	SystemInit();  
	Delay_Init();	
	Initialization();  // 系统初始化（包含OLED、按键、ESP8266、任务系统等
	
	uint32_t start_time = Task_GetSystemTime();
	
	//变量
	Serial_SendString("mode");
	//static uint32_t last_temp_read = 0;
	
	while(1)
	{	
		uint32_t current_time = Task_GetSystemTime();
		
		
		if( Get_Key_2() == 2 ){
			// 按键2：手动更新WiFi连接状态
			if(strstr(saved_ip, "172.168") || strstr(saved_ip, "192.168")) {
				Serial_SendString("按键2按下 - 手动更新WiFi连接状态...\r\n");
				connect_flag = 1;
				strcpy(saved_status, "已连接");
				Serial_SendString("WiFi连接状态已更新为：已连接\r\n");
			} else {
				Serial_SendString("按键2按下 - 但未检测到有效IP地址\r\n");
				// menu2();  // 如果需要原来的菜单功能，取消注释这行
			}
		}
		
		// 按键1连接巴法云
		if(Get_Key_1() == 1) {
			Serial_SendString("按键1按下 - 尝试连接巴法云...\r\n");
			Connect_To_BaFa_Cloud();
		}
		
		// 按键3手动设置巴法云连接状态（长按或多次按）
		static uint8_t key3_count = 0;
		if(Get_Key_3() == 3) {
			key3_count++;
			if(key3_count >= 3) {  // 连续按3次按键3
				Serial_SendString("按键3连按3次 - 手动设置巴法云连接状态...\r\n");
				if(connect_flag == 1) {
					connect_flag = 2;  // 设置为巴法云已连接状态
					Serial_SendString("巴法云连接状态已手动设置为：已连接 (connect_flag=2)\r\n");
					Serial_SendString("现在可以尝试上传数据了\r\n");
				} else {
					Serial_SendString("当前状态: ");
					Serial_SendNumber(connect_flag, 1);
					Serial_SendString(" - 请先确保WiFi连接正常\r\n");
				}
				key3_count = 0;  // 重置计数
			}
		} else {
			// 如果没有按键3，重置计数
			if(key3_count > 0) key3_count--;
		}
		
		// 按键4手动上传数据
		if(Get_Key_4() == 4) {
			Serial_SendString("\r\n=== 手动上传数据测试 ===\r\n");
			Serial_SendString("当前温度: ");
			Serial_SendNumber((uint32_t)data.Variable.Temp, 2);
			Serial_SendString("°C\r\n");
			Serial_SendString("当前湿度: ");
			Serial_SendNumber((uint32_t)data.Variable.Humi, 2);
			Serial_SendString("%\r\n");
			Serial_SendString("开始上传...\r\n");
			upload_data();
			Serial_SendString("========================\r\n");
		}
		
		Manual_Mode();
		data.flag.x++;
		Task_RunScheduler();
	}
}

