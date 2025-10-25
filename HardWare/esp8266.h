#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f10x.h"                  // Device header
#include "MYRTC.h"
#include "main.h"
#include "stdlib.h"
#include <ctype.h>    // 用于isdigit校验
#include "stdio.h"
#include "string.h"
#include "Delay.h"

#define CMD_WIFI "CMD:CONNECTED_WIFI}"					//检查wifi连接
#define CMD_GET_TIME "CMD:GET_TIME}"					//获取时间
#define CMD_GET_WIFI_INFO "CMD:GET_WIFI_INFO}"			//获取WiFi详细信息（名称和IP）
#define CMD_CONNECT_ALI "CMD:CONNECTED_MQTT"			//阿里云连接
#define CMD_CONNECT_BAFA "CMD:CONNECTED_TCP"			//巴法云连接
#define CMD_CONNECT_LOCAL "CMD:CONNECTED_HTTP"			//本地HTTP服务器
#define CMD_SEND_DATA "CMD:SEND_DATA"      				// 单片机发送数据到服务器的指令
#define CMD_FORGET_WIFI "CMD:FORGET_WIFI}"  			// 忘记WiFi密码指令

/********************巴法云密钥与主题******************/
#define BAFA_PRODUCT_ID   "51d78bea923947ebbeace0075d96cc21"  			//巴法云密钥
#define BAFA_TOPIC_CONTROL "tang1"  		// 上传数据主题（单片机→APP）
#define BAFA_TOPIC_DATA "tang2"    			// 订阅控制主题（APP→单片机）
#define BAFA_SERVER_PORT  "8344"

/*****************阿里云三元组**************/
#define ALI_PRODUCT_KEY   "k1r6clfL6U3"  			// 产品Key
#define ALI_DEVICE_NAME "fire"				// 设备名							
#define ALI_DEVICE_SECRET "cf00eba91c9437d2f4e8541e9b797ea4"			// 设备密钥
#define ALI_REGION_ID "cn-shanghai"				// 区域ID

#define RXMAX  512

#define BUFFER_SIZE 14  // 最大IP地址长度（含终止符）

extern uint8_t UART_RxFlag;
extern char HTTPIP[BUFFER_SIZE];
extern u8 connected;
extern uint8_t connect_flag;
extern char saved_wifi_name[50] ;
extern char saved_ip[20] ;

typedef struct{
	uint8_t rxbuff[RXMAX];//接收数据缓存
	uint16_t rxcount;   //接收的数量
	uint8_t rxover;   //接收的数量
	uint8_t txbuff[RXMAX];//接收数据缓存
}__MESSAGE;

extern __MESSAGE wifi;

void ESP8266_Config(void);												//串口初始化
void USART2_SendByte(uint8_t data);									//串口发送数据
void USART2_SendArray(uint8_t *Array, uint16_t Length);					//串口发送多数据
void WifiSendStr(char *p);												//发送字符串
void WifiSendbuff(uint8_t *p,uint8_t lenth);							//发送一定长度的字符串
void Clear_BuffData(void);												//清空缓冲区
void Check_the_network(void);											//检查网络连接状态
void Connect_tothe_cloud(char* x);										//连接到云端
void WIFI_GET_TIME(void);												//从esp8266获取时间
void WIFI_FORGET_WIFI(void);											//遗忘esp8266当前连接的wifi
void WIFI_GET_INFO(void);												//获取WiFi详细信息
void DataAnylize(void);													//数据接收处理
void Extract_WiFi_Info(void);                                          // 提取WiFi名称和IP地址
#endif
