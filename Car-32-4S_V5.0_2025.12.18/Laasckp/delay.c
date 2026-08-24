#include "delay.h"//延时函数头文件

//Systick定时器的使用参考网站：https://jishuzhan.net/article/1802100215709175810

void Delay_Init(void)//初始化延时函数	   
{	   
   SysTick->CTRL&=0xfffffffb;//控制寄存器，选择外部时钟即系统时钟的八分之一（HCLK/8；72M/8=9M）	   
 }

void delay_us(u32 Nus)//1us延时函数	   
{   	  
 SysTick->LOAD=Nus*9;//写入自动重装时间
 SysTick->VAL=0X00000000;//清空计数器	
 SysTick->CTRL|=0x00000001;//使能，减到零是无动作，采用外部时钟源；      
 while(!(SysTick->CTRL&(1<<16)));//等待时间到达，也就是判断第16位是否为1； 
 SysTick->CTRL=0X00000000;//关闭计数器   
 SysTick->VAL=0X00000000;//清空计数器   
 }

void delay_ms(u32 Nms)//1ms延时函数
{ 
 while(Nms--)//后自减
{
 delay_us(1000);//1000us延时
 }
 }
