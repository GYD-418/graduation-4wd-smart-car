/******************************************************

  作者：Laasckp	
  时间：2025.12.18
  版本：Car-32-4S_V5.0

  功能模式：1.两路红外避障蜂鸣器led报警模式       
            2.两路红外循迹模式
            3.速度和总里程测量模式
            4.超声波舵机旋转避障模式
            5.蜂鸣器单独鸣叫模式
            6.电池电压测量模式
            7.OLED显示屏显示功能
            8.PWM速度调节功能
            9.红外遥控控制功能
					 10.红绿蓝三色灯闪烁功能
           11.小程序，安卓APP蓝牙控制功能
   
  汇总程序：请参考interface.c
  接口定义：请参考interface.h 
 			
******************************************************/


#include "interface.h"//模块接口定义头文件

    int main(void)
 { 	 	 
	 All_Init();//所有想要初始化的函数	
	
    while(1)
  {	 
		 
	 stop_choice();//程序定时执行函数
		
	 stop_display_execute();//stop display execute
		
	 speed_set_display_execute();//speed set display execute
	  
	 decode_choice();//?????????????????????????
	 
	 GPS_Process();//GPS receive and parse
	 GPS_display_execute();//GPS display execute
	    		  
	 search_display_execute();//??·??????????????к???  
	  
	 obstacle_avoidance_display_execute();//??????????????к???	
	 
	    		  
	  
	  
		
   }	
 
 }


	
	

