
#include "speed_set.h"//速度设置头文件

unsigned char sudu=90;//小车速度设置为90 
unsigned char speed_and_flag=0;//小车速度设置加标志位
unsigned char speed_subtract_flag=0;//小车速度设置减标志位
 
 void speed_and()//小车速度加运算函数
 {	
	 sudu+=1;//小车速度每次加1
	 
	if(sudu>=100)//小车速度最大为100
	{
		sudu=100;   
	}	
	
 }

 void speed_subtract()//小车速度减运算函数
 {
	 
	sudu-=1;//小车速度每次减1     
	
	if(sudu<=15)//小车速度最小为15
	{
		sudu=15; 
	}
	
 }
 
 void speed_set_display_execute()//小车设置执行和显示函数
 {
	 
		if(speed_and_flag==1) //小车加标志等于1
							
			{					
				speed_and();//小车速度加运算函数
				OLED_ShowNum(80,6,sudu,3,0);//显示当前设置速度
				speed_and_flag=0;//小车速度加标志位置零		
			  IrDA=0;
			}
			
			if(speed_subtract_flag==1)//小车速度减标志位等于1
							
			{				
				speed_subtract();//小车速度减运算函数
				OLED_ShowNum(80,6,sudu,3,0);//显示当前设置速度百分比
				speed_subtract_flag=0;//小车速度减标志位置零
       	IrDA=0;	//	
			}		
			 
 }


