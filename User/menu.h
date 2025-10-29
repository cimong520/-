#ifndef __MENU_H
#define __MENU_H

int menu1(void);
int Blood_Oxygen_Threshold(void);
int Heart_Rate_Threshold(void);
int Temperature_Threshold(void);
int Blood_Oxygen_Low_set(void);
int Blood_Oxygen_Up_set(void);
int Heart_Rate_Low_set(void);
int Heart_Rate_Up_set(void);
int Temperature_Low_set(void);
int Temperature_Up_set(void);
int Humidity_Threshold(void);
int Humidity_Low_set(void);
int Humidity_Up_set(void);
int Soil_Threshold(void);   // 土壤湿度阈值显示
int Soil_Low_set(void);
int Soil_Up_set(void);
void Manual_Mode(void);
int Light_Threshold(void);
int Light_Low_set(void);
int Light_Up_set(void);

extern float Temperature_Lower ;  // 温度下限
extern float Temperature_Upper ;  // 温度上限
extern int Humidity_Lower ;       // 湿度下限
extern int Humidity_Upper ;       // 湿度上限
extern uint8_t mode_flag;
extern uint8_t key_flag;

#endif
