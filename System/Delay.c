#include "Delay.h"
#include "task.h"

//us延时
void Delay_us(uint64_t time)
{
	while(time--)
	{
		Delay_1us();
	}
}

//ms延时
void Delay_ms(uint64_t time)
{
	uint32_t start = Task_GetSystemTime();
    while((Task_GetSystemTime() - start) < time) {
        // 空循环等待
    }
}

void Delay_Init(void)
{
	if(SysTick_Config(72000) == 1)//定时1ms
	{
		while(1);
	}
}

//1ms发生中断1次
void SysTick_Handler(void)
{
	Task_UpdateTick();
}
