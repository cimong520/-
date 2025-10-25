#ifndef __INITIALIZATION_DATA_H__
#define __INITIALIZATION_DATA_H__

#include "../Library/stm32f10x_gpio.h"
/************************** 屏幕 连接引脚定义********************************/
#define OLED_SCL_PIN 				GPIO_Pin_6
#define OLED_SDA_PIN 				GPIO_Pin_7
#define OLED_GPIO					GPIOB
#define OLED_IIC					I2C1
#define OLED_GPIO_Clock 			RCC_APB2Periph_GPIOB
#define OLED_IIC_Clock  			RCC_APB1Periph_I2C1

/* 选择I2C类型: 0=软件I2C, 1=硬件I2C */
#define OLED_I2C_TYPE 1

/* 硬件I2C是否使用DMA: 0=不使用, 1=使用 */
#define OLED_USE_DMA 1
/************************** 屏幕 连接引脚定义********************************/

/************************** LED灯 连接引脚定义********************************/
#define LED_PIN 						GPIO_Pin_0
#define LED_GPIO						GPIOA
#define LED_GPIO_Clock 			RCC_APB2Periph_GPIOA
/************************** LED灯 连接引脚定义********************************/

/************************** 按键 连接引脚定义********************************/
#define KEY1_GPIO_PORT     GPIOA
#define KEY1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define KEY1_GPIO_PIN      GPIO_Pin_15

#define KEY2_GPIO_PORT     GPIOB
#define KEY2_GPIO_CLK      RCC_APB2Periph_GPIOB
#define KEY2_GPIO_PIN      GPIO_Pin_3

#define KEY3_GPIO_PORT     GPIOB
#define KEY3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define KEY3_GPIO_PIN      GPIO_Pin_4

#define KEY4_GPIO_PORT     GPIOB
#define KEY4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define KEY4_GPIO_PIN      GPIO_Pin_5
/************************** 按键 连接引脚定义********************************/

#endif
