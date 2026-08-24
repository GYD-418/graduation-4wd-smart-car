#include "interface.h"//模块接口定义头文件

void Peripheral_Clock_Init(void)//所有想要初始化的外设时钟函数
{
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA端口的时钟
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB端口的时钟
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能GPIOC端口的时钟
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//GPS UART1 clock
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能定时器2的时钟 
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//使能定时器4的时钟
	
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能串口3的时钟	
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
}

	
void All_Init(void)//所有想要初始化的函数
{	  
	Peripheral_Clock_Init();//所有想要初始化的外设时钟函数
	
	Delay_Init();  //初始化延时函数	
	Led_Init();    //LED初始化函数	
	IRCtrolInit(); //外部中断配置和遥控配置函数
	TIM2_Init();   //定时器2初始化函数
	motor_init();  //电机驱动初始化函数	
 
  search_init(); //红外循迹初始化函数	
	obstacle_avoidance_init();//红外避障初始化函数
	measure_init();           //真实速度测量初始化函数
 	 	
	ServoInit();      //舵机初始化函数
	ultrasonic_init();//超声波初始化函数
	DuojiMid();       //舵机居中函数
  buzzer_init();    //蜂鸣器初始化函数
	voltage_init();   //电池电压引脚采集引脚初始化	 
	adc1_init();      //ADC1初始化函数
  OLED_Init();   //OLED屏幕初始化函数
	
  OLED_ShowCH(15,1,"遥控选择模式");//显示 遥控选择模式 文字
  OLED_ShowCH(3,6,"设置速度:");    //设置 速度 文字
  OLED_ShowCH(110,6,"%");          //显示 % 符号
  OLED_ShowNum(80,6,sudu,3,0);     //显示 默认设置速度 数值
	
	USART2Conf(9600);         //USART2 init 9600
	USART3Conf(9600);         //BT UART3 init 9600
	USART1Conf(38400);        //GPS UART1 init 38400
	GPS_Init();               //GPS buffer init
	
}
	 
void stop_choice(void)//程序定时执行函数
{
 if(Time_1ms >= 5)//5ms的周期
 {
	Time_1ms=0;
	Time_5ms++;
	 
  led_execute();//led执行函数	
	ultrasonic_display_execute();//超声波舵机避障显示函数 			
			    			
 if(IrDA==0x18||IrDA==0x52||IrDA==0x5A||IrDA==0x08)//红外遥控控制前进，后退，左转，右转，才有效;
 {
	  continue_time--;//无接收指令就停车
		 
	  if(continue_time==0)
		 {
			continue_time=1;
		  CarStop();//四轮小车滑行停止 
			IrDA=0;//清零					
			}				
	}		      
	}
 
 if(Time_5ms>30)//5x30=150ms的周期
	{
	 Time_5ms = 0;		
	 speed_measurement_display_execute();//速度测量显示和执行函数     
	 buzzer_display_execute();           //蜂鸣器显示和执行函数
	 total_distance_display_execute();   //总里程测量显示和执行函数
	 battery_voltage_display_execute();  //电池电压显示和执行函数		 
	 }     	
}

void decode_choice(void)//红外遥控，蓝牙控制，选择功能函数	  
{		 
 if(ir_rec_flag==1|BT_flag==1)//接收到红外遥控信号或者接收到蓝牙信号				
 {		 
	if(ir_rec_flag==1)//接收到红外遥控信号						
	{						 
		IRIntIsr();   //红外遥控发射数据解码函数
    ir_rec_flag=0;//红外遥控标志位置零	           					
	 }		 
    BT_flag=0; //蓝牙标志位置零			  	 					
	 switch(IrDA)//红外遥控值或者蓝牙值选择执行对应的操作				 
	 {										
		case 0x18: CarGo();   //四轮小车前进 红外遥控上箭头键;   
 					     break;
					
		case 0x52: CarBack(); //四轮小车后退 红外遥控下箭头键;  
					     break;
					
		case 0x08: CarLeft(); //四轮小车左转 红外遥控左箭头键;  
					     break;
					
		case 0x5A: CarRight();//四轮小车右转 红外遥控右箭头键; 
					     break;
					 
		case 0x61: CarGo();   //四轮小车前进    蓝牙控制;   
 					     break;
					
		case 0x62: CarBack(); //四轮小车后退    蓝牙控制;  
					     break;
					
		case 0x63: CarLeft(); //四轮小车左转    蓝牙控制; 
					     break;
					
		case 0x64: CarRight();//四轮小车右转    蓝牙控制; 
					     break;
										 
		case 0x00: if(speed_measurement_flag==0)CarStop();//四轮小车滑行停止 蓝牙控制;
					     break;    
					
		case 0x1C: CarStop(); stop_display_flag=1;obstacle_avoidance_flag=0;search_flag=0;
				       speed_measurement_flag=0;total_distance_flag=0;ultrasonic_flag=0; buzzer_flag=0; gps_flag=0;
					     gps_display_flag=0;BUZZER_RESET;battery_voltage_flag=0;
					     break;//四轮小车滑行暂停，所有模式停止运行 红外遥控ok键  蓝牙控制;	
					
		case 0x45: obstacle_avoidance_flag=1;obstacle_avoidance_display_flag=1;search_flag=0;
					     speed_measurement_flag=0;ultrasonic_flag=0; buzzer_flag=0;
					     BUZZER_RESET;total_distance_flag=0;battery_voltage_flag=0;gps_flag=0;gps_flag=0;
					     break; //两路红外避障模式                  红外遥控1键   蓝牙控制;
					
		case 0x46: search_flag=1; search_display_flag=1;obstacle_avoidance_flag=0;
					     speed_measurement_flag=0;ultrasonic_flag=0; buzzer_flag=0;
							 BUZZER_RESET;total_distance_flag=0;battery_voltage_flag=0;gps_flag=0;gps_flag=0;
							 break;//两路红外循迹模式，                 红外遥控2键   蓝牙控制;
					
		case 0x47: speed_measurement_flag=1; speed_measurement_display_flag=1;
					     total_distance_flag=1;total_distance_display_flag=1;
               search_flag=0; obstacle_avoidance_flag=0;
					     ultrasonic_flag=0; buzzer_flag=0;
							 gps_display_flag=0;BUZZER_RESET;battery_voltage_flag=0;
							 break;//四轮小车测速和总里程测量模式       红外遥控3键   蓝牙控制;
													
		case 0x44: ultrasonic_flag=1;ultrasonic_display_flag=1;search_flag=0; 
					     obstacle_avoidance_flag=0; speed_measurement_flag=0; buzzer_flag=0;
							 BUZZER_RESET;total_distance_flag=0;battery_voltage_flag=0;gps_flag=0;gps_flag=0;
							 break;//超声波舵机旋转避障模式             红外遥控4键   蓝牙控制;	

    case 0x40: buzzer_flag=1;buzzer_display_flag=1;ultrasonic_flag=0;search_flag=0; 
					     obstacle_avoidance_flag=0; speed_measurement_flag=0;total_distance_flag=0;
 							 battery_voltage_flag=0;gps_flag=0;
							 break;//蜂鸣器鸣叫模式                      红外遥控5键  蓝牙控制;	
												 
		case 0x43: battery_voltage_flag=1;battery_voltage_display_flag=1; buzzer_flag=0;
					     ultrasonic_flag=0;search_flag=0;obstacle_avoidance_flag=0; 
							 speed_measurement_flag=0;total_distance_flag=0; BUZZER_RESET; gps_flag=0; 
							 break;//电池电压测量模式                    红外遥控6键  蓝牙控制;		
       					
		case 0x07: gps_flag=0;gps_display_flag=1;battery_voltage_flag=0;buzzer_flag=0;
				     ultrasonic_flag=0;search_flag=0;obstacle_avoidance_flag=0; 
					 speed_measurement_flag=0;total_distance_flag=0; BUZZER_RESET;
					 break;//GPS display mode                key 7;
		case 0x16: speed_and_flag=1;       
					     break;//设置速度加并显示当前设置速度功能    红外遥控*键  蓝牙控制;	
					
		case 0x0D: speed_subtract_flag=1; 
					     break;//设置速度减并显示当前设置速度功能    红外遥控#键  蓝牙控制;	
										 																								
		default  : break;	//不匹配，退出switch;				
				}							 				
			}	
    }

