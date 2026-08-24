#ifndef __STEERING_ENGINE_H_
#define __STEERING_ENGINE_H_

#include "interface.h"//模块接口定义头文件

extern unsigned int duoji_count;
extern unsigned int zhuanjiao;

void ServoInit(void);
void DuojiMid(void);
void DuojiRight(void);
void DuojiLeft(void);

#endif
