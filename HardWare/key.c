#include "../System/Initialization_Data.h"
#include "./key.h"
#include "../System/Delay.h"

uint8_t Get_Key_1(void)
{
    uint8_t KeyNum = 0;
    if (GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0);
        Delay_ms(20);
        KeyNum = 1;
    }
    return KeyNum;
}

uint8_t Get_Key_2(void)
{
    uint8_t KeyNum = 0;
    if (GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0);
        Delay_ms(20);
        KeyNum = 2;
    }
    return KeyNum;
}

uint8_t Get_Key_3(void)
{
    uint8_t KeyNum = 0;
    if (GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == 0);
        Delay_ms(20);
        KeyNum = 3;
    }
    return KeyNum;
}

uint8_t Get_Key_4(void)
{
    uint8_t KeyNum = 0;
    if (GPIO_ReadInputDataBit(KEY4_GPIO_PORT, KEY4_GPIO_PIN) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(KEY4_GPIO_PORT, KEY4_GPIO_PIN) == 0);
        Delay_ms(20);
        KeyNum = 4;
    }
    return KeyNum;
}    

uint8_t Key_Scan(void)
{
    if (GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0);
        Delay_ms(20);
        return 1;
    }
    
    if (GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0);
        Delay_ms(20);
        return 2;
    }
    
    if (GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == 0);
        Delay_ms(20);
        return 3;
    }
    
    if (GPIO_ReadInputDataBit(KEY4_GPIO_PORT, KEY4_GPIO_PIN) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(KEY4_GPIO_PORT, KEY4_GPIO_PIN) == 0);
        Delay_ms(20);
        return 4;
    }
    
    return 0;  // 没有按键按下
}

