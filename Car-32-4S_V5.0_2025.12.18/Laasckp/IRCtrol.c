#include "IRCtrol.h"//红外遥控头文件

unsigned char ir_rec_flag=0;//红外头接收数据标志位 1有数据 0没有
unsigned char IRCOM[4];//储存红外遥控发射数据
unsigned char continue_time=0;//停车时间
char IrDA=0;//红外控制指令

//外部中断配置 红外遥控配置
void IRCtrolInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;//定义一个外部中断相关的结构体
	NVIC_InitTypeDef NVIC_InitStructure; //定义一个中断的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	GPIO_InitStructure.GPIO_Pin = IRIN_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_Init(IRIN_GPIO , &GPIO_InitStructure);
		
	GPIO_EXTILineConfig(IRIN_PORTSOURCE , IRIN_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = IRIN_EXITLINE;//将对应的GPIO口连接到中断线上
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//中断事件类型，下降沿
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//选择模式，中断型
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能该中断
	EXTI_Init(&EXTI_InitStructure);//将配置好的参数写入寄存器
			
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2，2位抢占优先级，2位响应优先级
	NVIC_InitStructure.NVIC_IRQChannel = 	IRIN_IRQCH;//打开PINA_8的外部中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//主优先级0，最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级，
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能该模块中断
	NVIC_Init(&NVIC_InitStructure);	//中断初始化，将结构体定义的数据执行
	
}

void IRIntIsr(void)//?????????????????
{
 unsigned char j,k,N=0;
 
 USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
 delay_ms(2);//???2ms
 
 if(IRIN==1) 
 { 
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	 return;
 }
 
 continue_time=30;//????????
                         
 while(!IRIN)//??IRIN?????????????9ms?????????????	 
 {delay_us(140);}
 
 for (j=0;j<4;j++)//???????????
 { 
  for (k=0;k<8;k++)//?????????8λ
  {
   while (IRIN)//??IRIN?????????????4.5ms?????????????    
	 {delay_us(140);}
	 
   while (!IRIN)//??IRIN???????   
	 {delay_us(140);}
		
     while(IRIN)//????IR???????
     {  
			delay_us(140);
      N++;           
    if (N>=30)
	 { 
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	 return;}//0.14ms???????????????
      }//???????????                
     IRCOM[j]=IRCOM[j]>>1;//???????λ????0??
     if (N>=8){IRCOM[j] = IRCOM[j]|0x80;}//???????λ????1??
         N=0;
  }
 }
 
	k = ~IRCOM[3];
   if (IRCOM[2]!=k)
   { 
	 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
     return;}
	 
   IrDA=IRCOM[2];//???
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}