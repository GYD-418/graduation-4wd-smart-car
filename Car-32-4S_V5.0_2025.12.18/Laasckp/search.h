#ifndef __SEARCH_H_
#define __SEARCH_H_

#include "interface.h"//模块接口定义头文件

extern unsigned char search_flag;             //红外循迹执行标志位
extern unsigned char search_display_flag;    //红外循迹显示和执行标志位

void search_execute(void);                 //红外循迹函数
void search_init(void);                   //红外循迹初始化函数
void search_display_execute(void);       //红外循迹显示和执行函数

#endif

