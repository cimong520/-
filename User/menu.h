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
void Manual_Mode( void );
//�ⲿ��������
extern uint8_t mode_flag;
extern uint8_t key_flag;

#endif
