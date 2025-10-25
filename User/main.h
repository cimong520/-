#ifndef _MAIN_H_
#define _MAIN_H_

#include "../Start/stm32f10x.h"
#include "math.h"
#include "../HardWare/OLED_Data.h"
#include "../Library/stm32f10x_iwdg.h"
/**********�����ṹ��************/
typedef struct 
{
	float Temp;
	float Humi;
	float MH_RD;    // 添加MH_RD字段
} var;
/*******************************/

/**********��ֵ�ṹ��************/
typedef struct 
{
	float Temp;
	float Humi;
} thresh;
/*******************************/

/**********��־λ�ṹ��**********/
typedef struct 
{
	u8 i;
	u8 mode;
	u8 key1;
	u8 key2;
	u8 key3;
	u8 key4;
	int x;
	int y;
	int z;
	u8 led;         // 添加led字段
	u16 motor_flag; // 添加motor_flag字段
} flg;
/*******************************/

typedef struct 
{
	var Variable;       // Variable ���͵ĳ�Ա������Ϊ var
	thresh Threshold;   // Threshold ���͵ĳ�Ա������Ϊ thresh
	flg flag;           // flag ���͵ĳ�Ա������Ϊ flg
} dt;

extern dt data;

void Task_Initialization(void);
/**********ģ�����ݽṹ��**********/
// �����ﶨ���ģ����Ҫʹ�õ����ݽṹ
// ���磺
// typedef struct {
//     float temperature;
//     float humidity;
// } Module_DHT11_Data;
/**********************************/

// ����ģ�����ݣ������Ҫ��
// ���磺extern Module_DHT11_Data dht11_data;
#endif
