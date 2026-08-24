#ifndef __Delay_H_
#define __Delay_H_

#include "interface.h"//模块接口定义头文件

void Delay_Init(void);//初始化延时函数
void delay_us(u32 Nus);//微秒延时函数
void delay_ms(u32 Nms);//毫秒延时函数

#endif

