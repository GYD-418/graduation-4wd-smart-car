#ifndef __LED_H_
#define __LED_H_

#include "interface.h"//模块接口定义头文件

void Led_Init(void);//LED初始化函数

void led_red_frequency(uint8_t led_red);    //红色LED闪烁频率函数
void led_green_frequency(uint8_t led_green);//绿色LED闪烁频率函数
void led_blue_frequency(uint8_t led_blue);  //蓝色LED闪烁频率函数

void led_execute(void);//led执行函数

#endif
