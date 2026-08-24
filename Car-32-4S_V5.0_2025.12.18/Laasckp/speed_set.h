#ifndef __SPEED_SET_H_
#define __SPEED_SET_H_

#include "interface.h"//模块接口定义头文件

extern unsigned char sudu;//小车速度变量
extern unsigned char speed_and_flag;//小车速度加标志位
extern unsigned char speed_subtract_flag;//小车速度减标志位

void speed_and(void);//小车速度加运算函数
void speed_subtract(void);//小车速度减 运算函数
void speed_set_display_execute(void);//小车速度显示和执行函数

#endif
