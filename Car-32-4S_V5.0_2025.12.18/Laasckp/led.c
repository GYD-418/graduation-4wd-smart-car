#include "led.h"//led头文件

unsigned char led_5ms=0;//LED闪烁时间

void Led_Init(void)//LED初始化函数
{
  GPIO_InitTypeDef  GPIO_InitStructure;//GPIO配置结构体，声明一个结构体，名字是GPIO_InitStructure，结构体原型由GPIO_InitTypeDef确定;
	 		 
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN;      //配置GPIOx引脚                          ，下面类似同理;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出模式                           ，下面类似同理;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO口输出驱动电路的响应速度为2Mhz ，下面类似同理;
	GPIO_Init(LED_RED_GPIO,&GPIO_InitStructure);    //根据设定参数初始化GPIOx端口            ，下面类似同理;	
	LED_RED_SET;//输出默认给高电平，led灭，下面同理;
	 
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_RED_GPIO,&GPIO_InitStructure); 
	LED_GREEN_SET;
	 
  GPIO_InitStructure.GPIO_Pin = LED_BLUE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_BLUE_GPIO,&GPIO_InitStructure); 
	LED_BLUE_SET;
	 	   
}

/**************************************************************************
函数功能：红色led闪烁
入口参数：闪烁频率 
返回值  ：无
**************************************************************************/
void led_red_frequency(uint8_t led_red)//红色led闪烁频率函数
{	     	  	 	
	static uint8_t led_red_temp;//定义静态变量，下面类似同理;
	
  if(led_red==0) LED_RED_SET;//led灭，下面类似同理;
	
	else if(++led_red_temp==led_red)LED_RED_GPIO->ODR ^=LED_RED_PIN,led_red_temp=0;//指定管脚输出异或，实现对应的led状态取反目，下面类似同理;		
 }

 
/**************************************************************************
函数功能：绿色led闪烁
入口参数：闪烁频率 
返回值  ：无
**************************************************************************/
void led_green_frequency(uint8_t led_green)//绿色led闪烁频率函数
{	     	  	 	
	static uint8_t led_green_temp;
	
  if(led_green==0) LED_GREEN_SET;
	
	else if(++led_green_temp==led_green)LED_GREEN_GPIO->ODR ^=LED_GREEN_PIN,led_green_temp=0;	
 }

 
/**************************************************************************
函数功能：蓝色led闪烁
入口参数：闪烁频率 
返回值  ：无
**************************************************************************/
void led_blue_frequency(uint8_t led_blue)//蓝色led闪烁频率函数
{	     	  	 	
	static uint8_t led_blue_temp;
	
  if(led_blue==0) LED_BLUE_SET;
	
	else if(++led_blue_temp==led_blue)LED_BLUE_GPIO->ODR ^=LED_BLUE_PIN,led_blue_temp=0;		
 }


/**************************************************************************************************************
 
函数功能：led执行函数
入口参数：无 
返回值  ：无
说明    ：1.红外避障模式，红外循迹模式，测速和总里程模式，超声波舵机旋转避障模式，
            蜂鸣器鸣叫模式，电压测量模式，这5种模式中至少有一种模式执行时，绿色led闪烁，红色led灭，蓝色led灭;
          2.否则，蓝色led闪烁，红色led灭，绿色led灭;
 
***************************************************************************************************************/ 
void led_execute(void)//led执行函数
{
 if(obstacle_avoidance_flag==0&&search_flag==0&&speed_measurement_flag==0&&total_distance_flag==0&&ultrasonic_flag==0&&buzzer_flag==0&&battery_voltage_flag==0)
	{
	  led_blue_frequency(70);//5x70=350ms，蓝色led闪烁频率函数		
    LED_RED_SET;//红色led灭 	
	  LED_GREEN_SET;//绿色led灭	  
	 }
	
	if(obstacle_avoidance_flag==1||search_flag==1||speed_measurement_flag==1||total_distance_flag==1||ultrasonic_flag==1||buzzer_flag==1||battery_voltage_flag==1)
	 {
		 led_green_frequency(70);//5x70=350ms，绿色led闪烁频率函数
     LED_RED_SET; //红色led灭 	
	   LED_BLUE_SET;//蓝色led灭	  
		}				
 }
