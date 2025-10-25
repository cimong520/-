#ifndef _MYRTC_H_
#define _MYRTC_H_
#include "stdint.h"
extern uint16_t MyRTC_Time[];

void MyRTC_SetTime(void);
void MyRTC_ReadTime(void);
void MyRTC_Init(void);
#endif

